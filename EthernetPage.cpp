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

	{
		{
			WContainerWidget *configmode_conteiner = datacolumn->addWidget(std::make_unique<WContainerWidget>());
			configmode_conteiner->setStyleClass("setting");
			WText *label = configmode_conteiner->addWidget(std::make_unique<WText>(tr("ethernet_mode")));
			label->setStyleClass("label");
			mode_combobox = configmode_conteiner->addWidget(std::make_unique<WComboBox>());
			mode_combobox->addItem(tr("DHCP"));
			mode_combobox->addItem(tr("static"));
		}

		static_mode_conteiner = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		{
			WContainerWidget *c = static_mode_conteiner->addWidget(std::make_unique<WContainerWidget>());
			c->setStyleClass("setting");
			WText *label = c->addWidget(std::make_unique<WText>(tr("ethernet_ipaddr")));
			label->setStyleClass("label");
			ip_lineedit = c->addWidget(std::make_unique<WLineEdit>());
		}
		{
			WContainerWidget *c = static_mode_conteiner->addWidget(std::make_unique<WContainerWidget>());
			c->setStyleClass("setting");
			WText *label = c->addWidget(std::make_unique<WText>(tr("ethernet_netmask")));
			label->setStyleClass("label");
			mask_lineedit = c->addWidget(std::make_unique<WLineEdit>());
		}
		{
			WContainerWidget *c = static_mode_conteiner->addWidget(std::make_unique<WContainerWidget>());
			c->setStyleClass("setting");
			WText *label = c->addWidget(std::make_unique<WText>(tr("gateway")));
			label->setStyleClass("label");
			gateway_lineedit = c->addWidget(std::make_unique<WLineEdit>());
		}
		{
			WContainerWidget *c = static_mode_conteiner->addWidget(std::make_unique<WContainerWidget>());
			c->setStyleClass("setting");
			WText *label = c->addWidget(std::make_unique<WText>(tr("DNS")));
			label->setStyleClass("label");
			dns_lineedit = c->addWidget(std::make_unique<WLineEdit>());
		}
	}
	WRegExpValidator *ip_validaor = std::make_shared<WRegExpValidator>("((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
	WRegExpValidator *ip_validaor = new WRegExpValidator(
			"((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
	ip_lineedit->setValidator(ip_validaor);
	ip_lineedit->setStyleClass("iplineedit");
	mask_lineedit->setValidator(ip_validaor);
	mask_lineedit->setStyleClass("iplineedit");
	gateway_lineedit = new WLineEdit();
	gateway_lineedit->setValidator(ip_validaor);
	gateway_lineedit->setStyleClass("iplineedit");
	dns_lineedit->setValidator(ip_validaor);
	dns_lineedit->setStyleClass("iplineedit");

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
		default:
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
