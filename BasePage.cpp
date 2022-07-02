/*
 * BasePage.cpp
 *
 *  Created on: 03.04.2012
 *      Author: bond
 */
#include "BasePage.h"
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>

WComboBox *buildConsoleComboBox(WContainerWidget *father)
{
	WComboBox *console_combobox = father->addWidget(std::make_unique<WComboBox>());
	console_combobox->addItem("");
	for (int i = 1; i <= 6; i++) {
		console_combobox->addItem(WString("{1}").arg(i));
	}
	return console_combobox;
}

//==============================================================================
//===================== BasePage ===========================================
//==============================================================================
BasePage::BasePage(): WContainerWidget()
{
	WContainerWidget *pagetitle_div = this->addWidget(std::make_unique<WContainerWidget>());
	pagetitle_div->setStyleClass("pagetitle");
	pagetitle_text = pagetitle_div->addWidget(std::make_unique<WText>());

	WContainerWidget *ep = this->addWidget(std::make_unique<WContainerWidget>());
	datacolumn = ep->addWidget(std::make_unique<WContainerWidget>());
	datacolumn->setStyleClass("datacolumn");
	ep->addWidget(std::make_unique<WBreak>());
	save_button = ep->addWidget(std::make_unique<WPushButton>(tr("Save")));
	ep->addWidget(std::make_unique<WBreak>());
	help_panel = ep->addWidget(std::make_unique<WPanel>());
	help_panel->setTitle(tr("Help"));
	help_text = help_panel->setCentralWidget(std::make_unique<WText>());
}

//==============================================================================
//===================== BaseServicePage ===========================================
//==============================================================================
BaseServicePage::BaseServicePage(std::string servicename): WContainerWidget()
{
	this->servicename = servicename;

	WContainerWidget *pagetitle_div = this->addWidget(std::make_unique<WContainerWidget>());
	pagetitle_div->setStyleClass("pagetitle");
	pagetitle_text = pagetitle_div->addWidget(std::make_unique<WText>());

	WContainerWidget *ep = this->addWidget(std::make_unique<WContainerWidget>());
	servicecontrol = ep->addWidget(std::make_unique<ServiceControlWidget>(servicename));
	datacolumn = ep->addWidget(std::make_unique<WContainerWidget>());
	datacolumn->setStyleClass("datacolumn");
	ep->addWidget(std::make_unique<WBreak>());
	save_button = ep->addWidget(std::make_unique<WPushButton>(tr("Save")));
	ep->addWidget(std::make_unique<WBreak>());
	help_panel = ep->addWidget(std::make_unique<WPanel>());
	help_panel->setTitle(tr("Help"));
	help_text = help_panel->setCentralWidget(std::make_unique<WText>());
}
