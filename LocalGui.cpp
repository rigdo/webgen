#include "LocalGui.h"
#include <Wt/WString.h>
#include <Wt/WMenu.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>

#include "Settings.h"
#include "EthernetPage.h"
#include "OpenVpnPage.h"
#include "WiFiPage.h"
#include "RebootPage.h"
#include "PasswordPage.h"

#include "Xmrig.h"
#include "Ethminer.h"
#include "SummaryPage.h"
#include "VtPage.h"
#if USE_NVML
#include "NvidiaPage.h"
#endif
#if USE_ROCM_SMI
#include "AmdPage.h"
#endif



static VtState *vts[6 + 1];

//==============================================================================
//===================== LocalGui ================================================
//==============================================================================
LocalGui::LocalGui(const std::string &settings_dir): WContainerWidget()
{
	SettingsDir *sd = new SettingsDir(settings_dir);

	setStyleClass("wrap");

	WContainerWidget *header = addWidget(std::make_unique<WContainerWidget>());
	header->setStyleClass("header");
	WContainerWidget *navigation = addWidget(std::make_unique<WContainerWidget>());
	navigation->setStyleClass("navigation");
	WStackedWidget *content = addWidget(std::make_unique<WStackedWidget>());
	content->setStyleClass("content");

	bool localgui = WApplication::instance()->environment().clientAddress() == "127.0.0.1";

	{
		// header
		if (localgui) {
			WContainerWidget *lang_box = header->addWidget(std::make_unique<WContainerWidget>());
			lang_box->setStyleClass("langbox");
			WPushButton *lang_en_button = lang_box->addWidget(std::make_unique<WPushButton>());
			lang_en_button->clicked().connect( this, &LocalGui::setLangEn );
			WPushButton *lang_ru_button = lang_box->addWidget(std::make_unique<WPushButton>());
			lang_ru_button->clicked().connect( this, &LocalGui::setLangRu );
			header->addWidget(std::make_unique<WText>(tr("control_panel_header_local")));
		}
		else{
			header->addWidget(std::make_unique<WText>(tr("control_panel_header_remote")));
		}

	}

	{
		//, Vertical
		WMenu *topmenu = navigation->addWidget(std::make_unique<WMenu>(content));
		topmenu->setStyleClass("menu");
		topmenu->setInternalPathEnabled();
		topmenu->setInternalBasePath("/");

		topmenu->addItem(tr("summary"), std::make_unique<SummaryPage>(sd));
#if USE_NVML
		topmenu->addItem(tr("nvidia_oc"), std::make_unique<NvidiaPage>(sd));
#endif
#if USE_ROCM_SMI
		topmenu->addItem(tr("amd_oc"), std::make_unique<AmdPage>(sd));
#endif
		topmenu->addItem(tr("ethernet"), std::make_unique<EthernetPage>(sd));

		topmenu->addItem(tr("wifi_client"), std::make_unique<WiFiPage>(sd));
		topmenu->addItem(tr("Password"), std::make_unique<PasswordPage>(sd));
		topmenu->addItem(tr("Reboot"), std::make_unique<RebootPage>());
		//	ZipGenPage *zipgen = new ZipGenPage(settings_dir);

		//OpenVpnPage *openvpn_page = new OpenVpnPage(sd,0);
//		topmenu->addItem(tr("openvpn"), openvpn_page);
		topmenu->addItem(tr("xmrig-cpu_title"), std::make_unique<Xmrig>(sd, "cpu", "xmrig0"));
		topmenu->addItem(tr("xmrig-amd_title"), std::make_unique<Xmrig>(sd, "amd", "xmrig-amd0"));
		topmenu->addItem(tr("xmrig-nvidia_title"), std::make_unique<Xmrig>(sd, "nvidia", "xmrig-nvidia0"));
		topmenu->addItem(tr("ethminer-amd"), std::make_unique<Ethminer>(sd, "amd", "ethminer-amd0"));
		topmenu->addItem(tr("ethminer-nvidia"), std::make_unique<Ethminer>(sd, "nvidia", "ethminer-nvidia0"));
//		topmenu->addItem(tr("zipgen"), zipgen);

		if (!localgui){
			for (int vt_idx = 1; vt_idx <= 6; vt_idx++) {
				VtState *vt_state = nullptr;
				if (!vts[vt_idx]) {
					vts[vt_idx] = new VtState();
					bool ok = vts[vt_idx]->init(vt_idx);
					if (ok)
						vt_state = vts[vt_idx];
				} else {
					vt_state = vts[vt_idx];
				}
				if (!vt_state)
					continue;
				topmenu->addItem(WString("vt{1}").arg(vt_idx), std::make_unique<VtPage>(vt_state));
			}
		}
	}
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
