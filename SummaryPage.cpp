/*
 * SummaryPage.cpp
 *
 *  Created on: 08.05.2018
 *      Author: bond
 */

#include "SummaryPage.h"

//==============================================================================
//===================== SummaryPage ===========================================
//==============================================================================
SummaryPage::SummaryPage(SettingsDir *sd) : BasePage()
{
	this->sd = sd;
	pagetitle_text->setText(tr("summary_title"));
	help_text->setText( tr("summary_help"));

	std::string start_time = "2018-05-08 19:47:01";
	unsigned uptime_hour = 0;
	unsigned uptime_min = 15;
	std::string now_time =  "2018-05-08 19:51:01";
	std::string rigname = "vega";

	WText *text = new WText(tr("summary_1").arg(start_time).arg(uptime_hour).
			arg(uptime_min).arg(now_time).arg(rigname));

	std::string wifi_state = "wifi_state_search";
	std::string wifi_ip = "0.0.0.0";
	WText *wifi_state_w = new WText(tr("summary_wifi").arg(tr(wifi_state)).arg(wifi_ip));

	std::string eth_state = "eth_state_link_up";
	std::string eth_ip = "0.0.0.0";
	WText *eth_state_w = new WText(tr("summary_eth").arg(tr(eth_state)).arg(eth_ip));

	datacolumn->addWidget( text );
	datacolumn->addWidget( wifi_state_w );
	datacolumn->addWidget( eth_state_w );

	save_button->hide();
	help_panel->hide();
}
