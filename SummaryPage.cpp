/*
 * SummaryPage.cpp
 *
 *  Created on: 08.05.2018
 *      Author: bond
 */

#include "SummaryPage.h"
#include <Wt/WTimer.h>
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
#include <unistd.h>

//==============================================================================
//===================== SummaryPage ===========================================
//==============================================================================
SummaryPage::SummaryPage(SettingsDir *sd):
		BasePage()
{
	this->sd = sd;
	pagetitle_text->setText(tr("summary_title"));

	text_w = datacolumn->addWidget(std::make_unique<WText>());
	wifi_state_w = datacolumn->addWidget(std::make_unique<WText>());
	eth_state_w = datacolumn->addWidget(std::make_unique<WText>());

	save_button->hide();
	help_panel->hide();

	WTimer *timer = new WTimer();
	timer->setInterval(std::chrono::milliseconds(2000));
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

std::string getIfaceIp(const std::string &ifname, std::string def_val)
{
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return def_val;
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));
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
	// There's a trick to using unix datagram sockets.
	// Unlike stream sockets (tcp or unix domain), datagram sockets need
	// endpoints defined for both the server AND the client.
	struct sockaddr_un client_addr;
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sun_family = AF_UNIX;
	static int req;
	req++;
	sprintf(client_addr.sun_path, "/tmp/web_wpa_ctrl_%d-%u", getpid(), req);
	ret = bind(sockfd, (struct sockaddr *) &client_addr, sizeof(client_addr));
	if (ret) {
		close(sockfd);
		return res;
	}

	struct sockaddr_un server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sun_family = AF_UNIX;
	sprintf(server_addr.sun_path, "/var/run/wpa_supplicant0/%s", "wlan0");
	ret = connect(sockfd, (struct sockaddr *) &server_addr,
			sizeof(server_addr));
	if (ret) {
		unlink(client_addr.sun_path);
		close(sockfd);
		return res;
	}
	ret = write(sockfd, "STATUS", 6);
	if (ret != 6) {
		unlink(client_addr.sun_path);
		close(sockfd);
		return res;
	}
	char buf[1024];
	int bytes = read(sockfd, buf, sizeof(buf) - 1);
	close(sockfd);
	unlink(client_addr.sun_path);
	if (bytes <= 0) {
		return res;
	}
	buf[bytes] = 0;

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
	return res;
}

std::string
wpaAt(const std::map<std::string, std::string> &st, const std::string& name,
		std::string def_val)
{
	auto ptr = st.find(name);
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

	std::string rigname = "unk";
	struct utsname un;
	int ret = uname(&un);
	if( ret == 0) {
		rigname = std::string(un.nodename);
	}

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
