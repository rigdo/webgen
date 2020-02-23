#include "LocalGui.h"
#include <Wt/WString>
#include <Wt/WMenu>
#include <Wt/WStackedWidget>
#include <Wt/WApplication>
#include <Wt/WEnvironment>

#include "Settings.h"
#include "EthernetPage.h"
#include "OpenVpnPage.h"
#include "WiFiPage.h"
#include "RebootPage.h"
#include "PasswordPage.h"

#include "Xmrig.h"
#include "Ethminer.h"
#include "SummaryPage.h"
#include "NvidiaPage.h"
#include "AmdPage.h"
#include "VtPage.h"

static VtState *vts[6 + 1];

//==============================================================================
//===================== LocalGui ================================================
//==============================================================================
LocalGui::LocalGui(std::string settings_dir): WContainerWidget()
{
	SettingsDir *sd = new SettingsDir(settings_dir);

	SummaryPage *summarry_page = new SummaryPage(sd);
	NvidiaPage *nvidia_page = new NvidiaPage(sd);
	AmdPage *amd_page = new AmdPage(sd);

	EthernetPage *ethernet_page = new EthernetPage(sd);
	//OpenVpnPage *openvpn_page = new OpenVpnPage(sd,0);
	WiFiPage *wfi_page = new WiFiPage(sd);
	PasswordPage *password_page = new PasswordPage( sd );
	RebootPage *reboot_page = new RebootPage();


	Xmrig *xmrig = new Xmrig(sd, "cpu", "xmrig0");
	Xmrig *xmrig_amd = new Xmrig(sd, "amd", "xmrig-amd0");
	Xmrig *xmrig_nvidia = new Xmrig(sd, "nvidia", "xmrig-nvidia0");
	Ethminer *ethminer_amd = new Ethminer(sd, "amd", "ethminer-amd0");
	Ethminer *ethminer_nvidia = new Ethminer(sd, "nvidia", "ethminer-nvidia0");
//	ZipGenPage *zipgen = new ZipGenPage(settings_dir);
	bool localgui = WApplication::instance()->environment().clientAddress() == "127.0.0.1";

	VtPage *vt_pages[6 + 1];
	if (!localgui){
		for (int vt_idx = 1; vt_idx <= 6; vt_idx++) {
			VtState *vt = 0;
			if (!vts[vt_idx]) {
				vts[vt_idx] = new VtState();
				bool ok = vts[vt_idx]->init(vt_idx);
				if (ok)
					vt = vts[vt_idx];
			} else {
				vt = vts[vt_idx];
			}
			vt_pages[vt_idx] = 0;
			if (vt) {
				vt_pages[vt_idx] = new VtPage(vt);
			}
		}
	}

	WContainerWidget *header;
	{
		header = new WContainerWidget();
		header->setStyleClass("header");

		if (localgui) {
			WPushButton *lang_en_button = new WPushButton("[en]");
			lang_en_button->clicked().connect( this, &LocalGui::setLangEn );
			WPushButton *lang_ru_button = new WPushButton("[ru]");
			lang_ru_button->clicked().connect( this, &LocalGui::setLangRu );

			WContainerWidget *lang_box = new WContainerWidget();

			lang_box->setStyleClass("langbox");
			lang_box->addWidget( lang_en_button );
			lang_box->addWidget( lang_ru_button );
			header->addWidget( lang_box );

			header->addWidget(new WText(tr("control_panel_header_local")));
		}
		else{
			header->addWidget(new WText(tr("control_panel_header_remote")));
		}

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

		topmenu = new WMenu(content, Vertical);
		topmenu->setStyleClass("menu");
		topmenu->setInternalPathEnabled();
		topmenu->setInternalBasePath("/");


		topmenu->addItem(tr("summary"), summarry_page);
		topmenu->addItem(tr("nvidia_oc"), nvidia_page);
		topmenu->addItem(tr("amd_oc"), amd_page);
		topmenu->addItem(tr("ethernet"), ethernet_page);
		topmenu->addItem(tr("wifi_client"), wfi_page);
		topmenu->addItem(tr("Password"), password_page);
		topmenu->addItem(tr("Reboot"), reboot_page);

//		topmenu->addItem(tr("openvpn"), openvpn_page);
		topmenu->addItem(tr("xmrig-cpu_title"), xmrig);
		topmenu->addItem(tr("xmrig-amd_title"), xmrig_amd);
		topmenu->addItem(tr("xmrig-nvidia_title"), xmrig_nvidia);
		topmenu->addItem(tr("ethminer-amd"), ethminer_amd);
		topmenu->addItem(tr("ethminer-nvidia"), ethminer_nvidia);
//		topmenu->addItem(tr("zipgen"), zipgen);
		if (!localgui){
			for (int i = 1; i <= 6; i++) {
				if (!vt_pages[i])
					continue;
				topmenu->addItem(WString("vt{1}").arg(i), vt_pages[i]);
			}
		}
		navigation->addWidget(topmenu);
	}

	setStyleClass("wrap");
	addWidget(header);
	addWidget(navigation);
	addWidget(content);
}

void LocalGui::setLangEn()
{
	WApplication *app = WApplication::instance();
	app->setLocale("en");
}

void LocalGui::setLangRu()
{
	WApplication *app = WApplication::instance();
	app->setLocale("ru");
}
