#include "LocalGui.h"
#include <Wt/WString>
#include <Wt/WMenu>
#include <Wt/WStackedWidget>
#include <Wt/WApplication>

#include "Settings.h"
#include "EthernetPage.h"
#include "OpenVpnPage.h"
#include "WiFiPage.h"
#include "Xmrig.h"
#include "XmrigNvidia.h"
#include "XmrigAmd.h"
#include "Ethminer.h"
#include "SummaryPage.h"
#include "NvidiaPage.h"
#include "VtPage.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

static VtState *vts[6+1];

//==============================================================================
//===================== LocalGui ================================================
//==============================================================================
LocalGui::LocalGui( std::string settings_dir) : WContainerWidget()
{
	SettingsDir *sd = new SettingsDir(settings_dir);
	
	SummaryPage *summarry_page = new SummaryPage(sd);
	//NvidiaPage *nvidia_page = new NvidiaPage(sd);

	EthernetPage *ethernet_page = new EthernetPage(sd);
	//OpenVpnPage *openvpn_page = new OpenVpnPage(sd,0);
	WiFiPage *wfi_page = new WiFiPage(sd);
	Xmrig *xmrig = new Xmrig(sd);
	XmrigAmd *xmrig_amd = new XmrigAmd(sd);
	XmrigNvidia *xmrig_nvidia = new XmrigNvidia(sd);
	Ethminer *ethminer_amd = new Ethminer(sd, "amd"); 
	Ethminer *ethminer_nvidia = new Ethminer(sd, "nvidia"); 
//	ZipGenPage *zipgen = new ZipGenPage(settings_dir);
	VtPage *vt_pages[6+1];
	for(int vt_idx=1; vt_idx<= 6; vt_idx++){
		VtState *vt = 0;
		if(!vts[vt_idx]){
			vts[vt_idx] = new VtState();
			bool ok = vts[vt_idx]->init(vt_idx);
			if(ok)
				vt = vts[vt_idx];
		}
		else{
			vt = vts[vt_idx];
		}
		vt_pages[vt_idx] = 0;
		if(vt){
			vt_pages[vt_idx] = new VtPage(vt);
		}
	}
		
	WContainerWidget *header;
	{
		header = new WContainerWidget();
		header->setStyleClass("header");
		header->addWidget( new WText(tr("control panel header")) );
		
//		WPushButton *lang_en_button = new WPushButton("en");
//		lang_en_button->clicked().connect( this, &LocalGui::setLangEn );
//		WPushButton *lang_ru_button = new WPushButton("ru");
//		lang_ru_button->clicked().connect( this, &LocalGui::setLangRu );
//
//		WContainerWidget *lang_box = new WContainerWidget();
//
//		lang_box->setStyleClass("langbox");
//		lang_box->addWidget( lang_en_button );
//		lang_box->addWidget( lang_ru_button );
//		header->addWidget( lang_box );
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
		

		topmenu->addItem(tr("summary"), summarry_page);
		//topmenu->addItem(tr("nvidia_oc"), nvidia_page);
		topmenu->addItem(tr("ethernet"), ethernet_page);
		topmenu->addItem(tr("wifi_client"), wfi_page);
//		topmenu->addItem(tr("openvpn"), openvpn_page);
		topmenu->addItem(tr("xmrig"), xmrig);
		topmenu->addItem(tr("xmrig-amd"), xmrig_amd);
		topmenu->addItem(tr("xmrig-nvidia"), xmrig_nvidia);
		topmenu->addItem(tr("ethminer-amd"), ethminer_amd);
		topmenu->addItem(tr("ethminer-nvidia"), ethminer_nvidia);
//		topmenu->addItem(tr("zipgen"), zipgen);
		for(int i=1;i<=6;i++){
			if(!vt_pages[i])
				continue;
			topmenu->addItem(WString("vt{1}").arg(i),vt_pages[i]);
		}
		navigation->addWidget( topmenu );
	}
	
	setStyleClass("wrap");
	addWidget( header );
	addWidget( navigation );
	addWidget( content );
}

void LocalGui::setLangEn()
{
	WApplication* app = WApplication::instance();
	app->setLocale( "en" );
}

void LocalGui::setLangRu()
{
	WApplication* app = WApplication::instance();
	app->setLocale( "ru" );
}
