/*
 * WiFiPage.cpp
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#include "WiFiPage.h"
#include <Wt/WRegExpValidator>

//==============================================================================
//===================== WiFiPage ===========================================
//==============================================================================
WiFiPage::WiFiPage(SettingsDir *sd) : BasePage()
{
	this->sd = sd;
	pagetitle_text->setText(tr("wifi_client"));
	help_text->setText( tr("wifi_client_help"));
		
	autostart_checkbox = new WCheckBox();
	ssid_lineedit = new WLineEdit();
	ssid_lineedit->setTextSize(49);
	ssid_lineedit->setStyleClass("settingvalue");
	psk_lineedit = new WLineEdit();
	psk_lineedit->setTextSize(49);
	psk_lineedit->setStyleClass("settingvalue");
	
	{
		{
		WContainerWidget *c = new WContainerWidget();
		c->setStyleClass("setting");
		WText *label = new WText(tr("wifi_autostart"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( autostart_checkbox );
		datacolumn->addWidget( c );
		}
		{
		WContainerWidget *c = new WContainerWidget();
		c->setStyleClass("setting");
		WText *label = new WText(tr("SSID"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( ssid_lineedit );
		datacolumn->addWidget( c );
		}
		{
		WContainerWidget *c = new WContainerWidget();
		c->setStyleClass("setting");
		WText *label = new WText(tr("PSK"));
		label->setStyleClass("label");
		c->addWidget( label );
		c->addWidget( psk_lineedit );
		datacolumn->addWidget( c );
		}
	}
	save_button->clicked().connect( this, &WiFiPage::saveParams );
	loadParams();
}

void WiFiPage::loadParams()
{
	Settings s = sd->byService("wpa0");
	
	ssid_lineedit->setText( s.value("WPA_SSID", "name") );
	psk_lineedit->setText( s.value("WPA_PSK", "passw") );
	
	autostart_checkbox->setChecked( s.valueInt("AUTOSTART", 0) );
}

void WiFiPage::saveParams()
{
	Settings s = sd->byService("wpa0");
	s.save( "WPA_SSID", ssid_lineedit->text() );
	s.save( "WPA_PSK", psk_lineedit->text() );
	s.saveInt( "AUTOSTART", autostart_checkbox->isChecked());
	Settings s2 = sd->byService("wpa_cli0");
	s2.saveInt( "AUTOSTART", autostart_checkbox->isChecked());
}
