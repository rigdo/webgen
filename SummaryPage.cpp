/*
 * SummaryPage.cpp
 *
 *  Created on: 08.05.2018
 *      Author: bond
 */

#include "SummaryPage.h"
#include <Wt/WTimer>
#include <time.h>
#include <sys/utsname.h>

#include <string.h> /* for strncpy */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <sys/un.h>

//==============================================================================
//===================== SummaryPage ===========================================
//==============================================================================
SummaryPage::SummaryPage(SettingsDir *sd):
		BasePage()
{
	this->sd = sd;
	pagetitle_text->setText(tr("summary_title"));

	text_w = new WText();
	wifi_state_w = new WText();
	eth_state_w = new WText();

	datacolumn->addWidget(text_w);
	datacolumn->addWidget(wifi_state_w);
	datacolumn->addWidget(eth_state_w);

	save_button->hide();
	help_panel->hide();

	WTimer *timer = new WTimer();
	timer->setInterval(2000);
	timer->timeout().connect(this, &SummaryPage::updateSummary);
	timer->start();

	updateSummary();
}

unsigned loadUptime()
{
	FILE *fp = fopen("/proc/uptime", "rt");
	if (!fp)
		return 0;
	unsigned uptime_sec;
	int ret = fscanf(fp, "%u", &uptime_sec);
	fclose(fp);
	if (ret != 1)
		return 0;
	return uptime_sec;
}

std::string nowDateTime()
{
	time_t rawtime;
	time(&rawtime);
	struct tm *info = localtime(&rawtime);
	char buf[128];
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", info);
	return std::string(buf);
}

int loadFromSysFileInt(const char *fname, int def_val)
{
	FILE *fp = fopen(fname, "rt");
	if (!fp)
		return def_val;
	int val;
	int ret = fscanf(fp, "%d", &val);
	fclose(fp);
	if (ret != 1)
		return def_val;
	return val;
}

std::string getIfaceIp(std::string ifname, std::string def_val)
{
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return def_val;
	struct ifreq ifr;
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, ifname.c_str(), IFNAMSIZ - 1);
	int ret = ioctl(fd, SIOCGIFADDR, &ifr);
	close(fd);
	if (ret != 0)
		return def_val;
	return std::string(
			inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr));
}

std::map<std::string, std::string> getWpaSupplicantStatus()
{
	std::map<std::string, std::string> res;
	int sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sockfd < 0)
		return res;

	int ret;
	struct sockaddr_un server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sun_family = AF_UNIX;
	sprintf(server_addr.sun_path, "/var/run/wpa_supplicant0/%s", "wlan0");
	ret = connect(sockfd, (struct sockaddr *) &server_addr,
			sizeof(server_addr));
	if (ret) {
		close(sockfd);
		return res;
	}
	ret = write(sockfd, "STATUS", 6);
	if (ret != 6) {
		close(sockfd);
		return res;
	}
	char buf[1024];
	int bytes = read(sockfd, buf, sizeof(buf));
	close(sockfd);
	printf("bytes: %s\n", buf);
	char *bufp = buf;
	char *saveptr1 = 0;
	do {
		char *line = strtok_r(bufp, "\n", &saveptr1);
		if (!line)
			break;
		bufp = 0;
		char *saveptr2 = 0;
		char *name = strtok_r(line, "=", &saveptr2);
		if (!name)
			continue;
		char *value = strtok_r(0, "=", &saveptr2);
		if (!value)
			continue;
		res[std::string(name)] = std::string(value);
	} while (1);
	close(sockfd);
	return res;
}

std::string
wpaAt(const std::map<std::string, std::string> &st, std::string name,
		std::string def_val)
{
	std::map<std::string, std::string>::const_iterator ptr = st.find(name);
	if (ptr == st.end())
		return def_val;
	return ptr->second;
}

void SummaryPage::updateSummary()
{

	std::string start_time = "";

	unsigned uptime_sec = loadUptime();
	unsigned uptime_day = uptime_sec / (3600 * 24);
	unsigned uptime_hour = (uptime_sec - uptime_day * 3600 * 24) / 3600;
	unsigned uptime_min =
			(uptime_sec - uptime_day * 3600 * 24 - uptime_hour * 3600) / 60;

	std::string now_time = nowDateTime();

	struct utsname un;
	uname(&un);
	std::string rigname(un.nodename);

	text_w->setText(tr("summary_1").arg(start_time).arg(uptime_day).arg(
			uptime_hour).arg(uptime_min).arg(now_time).arg(rigname));

	std::map<std::string, std::string> st = getWpaSupplicantStatus();
	std::string
			wifi_state = wpaAt(st, "wpa_state", "NODEV"); // COMPLETED, SCANNING
	std::string wifi_ip = getIfaceIp("wlan0", "");
	wifi_state_w->setText(tr("summary_wifi").arg(tr(wifi_state)).arg(wifi_ip));

	int carrier = loadFromSysFileInt("/sys/class/net/eth0/carrier", 0);
	std::string eth_state = "eth_state_link_down";
	if (carrier)
		eth_state = "eth_state_link_up";
	std::string eth_ip = getIfaceIp("eth0", "");
	eth_state_w->setText(tr("summary_eth").arg(tr(eth_state)).arg(eth_ip));
}

