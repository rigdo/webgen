#include "SiteGui.h"
#include <Wt/WSubMenuItem>
#include <Wt/WString>
#include <Wt/Http/Response>
#include <Wt/WMenu>
#include <Wt/WStackedWidget>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

//==============================================================================
//===================== ZipGen ================================================= 
//==============================================================================
ZipGen::ZipGen(std::string settings_dir)
{
	this->settings_dir = settings_dir;
	suggestFileName("rigdo_settings.zip");
}

ZipGen::~ZipGen()
{
	beingDeleted(); // see "Concurrency issues" below.
}

void ZipGen::handleRequest(const Wt::Http::Request& request,
		Wt::Http::Response& response)
{
	response.setMimeType("application/zip");
		
	char cmd[ 1024 ];
	snprintf(cmd, 1023, "cd %s/.. && zip -r - settings", settings_dir.c_str());
	printf("run : '%s'\n", cmd);

	FILE *fp = popen(cmd, "r");
	if (!fp) {
		printf("can't do popen for cmd: \"%s\"\n", cmd);
		response.setStatus(500);
		return;
	}
	int maxsize = 100 * 1024;
	char *buf = new char[maxsize];
	while (!feof(fp)) {
		int ret = fread(buf, 1, maxsize, fp);
		if (ret <= 0)
			break;
		response.out().write(buf, ret);
	}
	delete[] buf;

	int retcode = pclose(fp);
	if (retcode != 0)
		response.setStatus(500);
}

//==============================================================================
//===================== SiteGui ================================================ 
//==============================================================================
SiteGui::SiteGui( std::string settings_dir) : WContainerWidget()
{
	SettingsDir *sd = new SettingsDir(settings_dir);
	
	EthernetPage *ethernet_page = new EthernetPage(sd);
	OpenVpnPage *openvpn_page = new OpenVpnPage(sd,0);
	WiFiPage *wfi_page = new WiFiPage(sd);
	Xmrig *xmrig = new Xmrig(sd);
	XmrigNvidia *xmrig_nvidia = new XmrigNvidia(sd);
	XmrigAmd *xmrig_amd = new XmrigAmd(sd);
	Ethminer *ethminer = new Ethminer(sd); 
	
	ZipGen *zip_gen = new ZipGen(settings_dir);
	
	WContainerWidget *header;
	{
		header = new WContainerWidget();
		header->setStyleClass("header");
		header->addWidget( new WText(tr("Header")) );
		WPushButton *zip_button = new WPushButton(tr("download_zip"));
		header->addWidget( zip_button );
		
		zip_button->setLink(WLink(zip_gen->url()));
	}
	
	WStackedWidget *content = new WStackedWidget();
	{
		content = new WStackedWidget();
		content->setStyleClass("content");
	}

	WContainerWidget *navigation;
	WMenu *topmenu;
	{
		navigation = new WContainerWidget();
		navigation->setStyleClass("navigation");
		
		topmenu = new WMenu( content, Vertical);
		topmenu->setStyleClass("menu");
		topmenu->setInternalPathEnabled();
		topmenu->setInternalBasePath("/");
		
		topmenu->addItem(tr("ethernet"), ethernet_page);
		topmenu->addItem(tr("wifi_client"), wfi_page);
	//	topmenu->addItem(tr("openvpn"), content->openvpn_page);
		topmenu->addItem(tr("xmrig"), xmrig);
		topmenu->addItem(tr("xmrig-nvidia"), xmrig_nvidia);
		topmenu->addItem(tr("xmrig-amd"), xmrig_amd);
		topmenu->addItem(tr("ethminer"), ethminer);
		navigation->addWidget( topmenu );
	}
	
	setStyleClass("wrap");
	addWidget( header );
	addWidget( navigation );
	addWidget( content );
}
