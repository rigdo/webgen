#include "SiteGui.h"
#include <Wt/WString.h>
#include <Wt/WMenu.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WApplication.h>

#include "Settings.h"
#include "EthernetPage.h"
#include "OpenVpnPage.h"
#include "WiFiPage.h"
#include "PasswordPage.h"
#include "Xmrig.h"
#include "Ethminer.h"
#include "ZipGenPage.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

//==============================================================================
//===================== SiteGui ================================================ 
//==============================================================================
SiteGui::SiteGui(std::string settings_dir):
		WContainerWidget()
{
	SettingsDir *sd = new SettingsDir(settings_dir);
	setStyleClass("wrap");

//	OpenVpnPage *openvpn_page = new OpenVpnPage(sd, 0);

	WContainerWidget *header = addWidget(std::make_unique<WContainerWidget>());
	header->setStyleClass("header");
	WContainerWidget *navigation = addWidget(std::make_unique<WContainerWidget>());
	navigation->setStyleClass("navigation");
	WStackedWidget *content = addWidget(std::make_unique<WStackedWidget>());
	content->setStyleClass("content");

	{
		WContainerWidget *lang_box = header->addWidget(std::make_unique<WContainerWidget>());
		lang_box->setStyleClass("langbox");
		WPushButton *lang_en_button = lang_box->addWidget(std::make_unique<WPushButton>());
		lang_en_button->clicked().connect( this, &SiteGui::setLangEn );
		WPushButton *lang_ru_button = lang_box->addWidget(std::make_unique<WPushButton>());
		lang_ru_button->clicked().connect( this, &SiteGui::setLangRu );
		header->addWidget(std::make_unique<WText>(tr("control_panel_header_local")));
	}


	{
		//, Vertical
		WMenu *topmenu = navigation->addWidget(std::make_unique<WMenu>(content));
		topmenu->setStyleClass("menu");
		topmenu->setInternalPathEnabled();
		topmenu->setInternalBasePath("/");

		topmenu->addItem(tr("ethernet"), std::make_unique<EthernetPage>(sd));
		topmenu->addItem(tr("wifi_client"), std::make_unique<WiFiPage>(sd));
		topmenu->addItem(tr("Password"), std::make_unique<PasswordPage>(sd));

//		topmenu->addItem(tr("openvpn"), openvpn_page);

		topmenu->addItem(tr("xmrig-cpu_title"), std::make_unique<Xmrig>(sd, "cpu", "xmrig0"));
		topmenu->addItem(tr("xmrig-amd_title"), std::make_unique<Xmrig>(sd, "amd", "xmrig-amd0"));
		topmenu->addItem(tr("xmrig-nvidia_title"), std::make_unique<Xmrig>(sd, "nvidia", "xmrig-nvidia0"));
		topmenu->addItem(tr("ethminer-amd"), std::make_unique<Ethminer>(sd, "amd", "ethminer-amd0"));
		topmenu->addItem(tr("ethminer-nvidia"), std::make_unique<Ethminer>(sd, "nvidia", "ethminer-nvidia0"));
		topmenu->addItem(tr("zipgen"), std::make_unique<ZipGenPage>(settings_dir));
	}
}

void SiteGui::setLangEn()
{
	WApplication *app = WApplication::instance();
	app->setLocale("en");
}

void SiteGui::setLangRu()
{
	WApplication *app = WApplication::instance();
	app->setLocale("ru");
}
