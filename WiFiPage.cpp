/*
 * WiFiPage.cpp
 *
 *      Author: bond
 */

#include "WiFiPage.h"

//==============================================================================
//===================== WiFiPage ===========================================
//==============================================================================
WiFiPage::WiFiPage(SettingsDir *sd):
		BasePage()
{
	this->sd = sd;
	pagetitle_text->setText(tr("wifi_client"));
	help_text->setText(tr("wifi_client_help"));

	{
		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("autostart")));
		label->setStyleClass("label");
		autostart_checkbox = c->addWidget(std::make_unique<WCheckBox>());
	}
	{
		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("SSID")));
		label->setStyleClass("label");
		ssid_lineedit = c->addWidget(std::make_unique<WLineEdit>());
		ssid_lineedit->setTextSize(49);
		ssid_lineedit->setStyleClass("settingvalue");
	}
	{
		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("PSK")));
		label->setStyleClass("label");
		psk_lineedit = c->addWidget(std::make_unique<WLineEdit>());
		psk_lineedit->setTextSize(49);
		psk_lineedit->setStyleClass("settingvalue");
	}
	save_button->clicked().connect(this, &WiFiPage::saveParams);
	loadParams();
}

void WiFiPage::loadParams()
{
	Settings s = sd->byService("wpa0");

	ssid_lineedit->setText(s.value("WPA_SSID", "name"));
	psk_lineedit->setText(s.value("WPA_PSK", "passw"));

	autostart_checkbox->setChecked(s.valueInt("AUTOSTART", 0));
}

void WiFiPage::saveParams()
{
	Settings s = sd->byService("wpa0");
	s.save("WPA_SSID", ssid_lineedit->text());
	s.save("WPA_PSK", psk_lineedit->text());
	s.saveInt("AUTOSTART", autostart_checkbox->isChecked());
	Settings s2 = sd->byService("wpa_cli0");
	s2.saveInt("AUTOSTART", autostart_checkbox->isChecked());
}
