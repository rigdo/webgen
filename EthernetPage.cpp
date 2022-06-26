/*
 * EthernetPage.cpp
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#include "EthernetPage.h"
#include <Wt/WRegExpValidator.h>

//==============================================================================
//===================== EthernetPage ===========================================
//==============================================================================
EthernetPage::EthernetPage(SettingsDir *sd):
		BasePage()
{
	this->sd = sd;
	pagetitle_text->setText(tr("ethernet"));
	help_text->setText(WString(tr("ethernet_help")).arg(tr("IP Address")));

	mode_combobox = new WComboBox();
	mode_combobox->addItem(tr("DHCP"));
	mode_combobox->addItem(tr("static"));


	ip_lineedit = new WLineEdit();
	WRegExpValidator *ip_validaor = new WRegExpValidator(
			"((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
	ip_lineedit->setValidator(ip_validaor);
	ip_lineedit->setStyleClass("iplineedit");
	mask_lineedit = new WLineEdit();
	mask_lineedit->setValidator(ip_validaor);
	mask_lineedit->setStyleClass("iplineedit");
	gateway_lineedit = new WLineEdit();
	gateway_lineedit->setValidator(ip_validaor);
	gateway_lineedit->setStyleClass("iplineedit");

	dns_lineedit = new WLineEdit();
	dns_lineedit->setValidator(ip_validaor);
	dns_lineedit->setStyleClass("iplineedit");

	{
		{
			WContainerWidget *configmode_conteiner = new WContainerWidget();
			configmode_conteiner->setStyleClass("setting");
			WText *label = new WText(tr("ethernet_mode"));
			label->setStyleClass("label");
			configmode_conteiner->addWidget(label);
			configmode_conteiner->addWidget(mode_combobox);
			datacolumn->addWidget(configmode_conteiner);
		}

		static_mode_conteiner = new WContainerWidget();
		datacolumn->addWidget(static_mode_conteiner);
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("ethernet_ipaddr"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(ip_lineedit);
			static_mode_conteiner->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("ethernet_netmask"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(mask_lineedit);
			static_mode_conteiner->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("gateway"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(gateway_lineedit);
			static_mode_conteiner->addWidget(c);
		}
		{
			WContainerWidget *c = new WContainerWidget();
			c->setStyleClass("setting");
			WText *label = new WText(tr("DNS"));
			label->setStyleClass("label");
			c->addWidget(label);
			c->addWidget(dns_lineedit);
			static_mode_conteiner->addWidget(c);
		}
	}
	save_button->clicked().connect(this, &EthernetPage::saveParams);
	mode_combobox->activated().connect(this, &EthernetPage::modeChanged);
	loadParams();
	modeChanged();
}

void EthernetPage::loadParams()
{
	Settings s = sd->byService("kernel");

	WString v = s.value("NETWORK_MODE", "dhcpc");
	mode_combobox->setCurrentIndex((v == "static") ? 1 : 0);

	ip_lineedit->setText(s.value("NETWORK_IP", "192.168.1.198"));
	mask_lineedit->setText(s.value("NETWORK_MASK", "255.255.255.0"));
	gateway_lineedit->setText(s.value("NETWORK_DEFAULT_GATEWAY", "0.0.0.0"));

	Settings s2 = sd->byService("userspace");
	dns_lineedit->setText(s2.value("DNS1", "8.8.8.8"));
}

void EthernetPage::saveParams()
{
	Settings s = sd->byService("kernel");

	if (mode_combobox->currentIndex() == 0) {
		s.save("NETWORK_MODE", "dhcpc");
	} else {
		s.save("NETWORK_MODE", "static");
		s.save("NETWORK_IP", ip_lineedit->text());
		s.save("NETWORK_MASK", mask_lineedit->text());
		s.save("NETWORK_DEFAULT_GATEWAY", gateway_lineedit->text());
		Settings s2 = sd->byService("userspace");
		s2.save("DNS1", dns_lineedit->text());
	}

}

void EthernetPage::modeChanged()
{
	int newval = mode_combobox->currentIndex();
	switch (newval) {
		case 0: //DHCP
			static_mode_conteiner->hide();
			help_text->setText(tr("ethernet_DHCP_help"));
			break;
		case 1: //Static
			static_mode_conteiner->show();
			help_text->setText(tr("ethernet_static_help"));
			break;
	}
}
