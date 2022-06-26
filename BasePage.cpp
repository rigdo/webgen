/*
 * BasePage.cpp
 *
 *  Created on: 03.04.2012
 *      Author: bond
 */
#include "BasePage.h"
#include <Wt/WBreak.h>

WComboBox *buildConsoleComboBox()
{
	WComboBox *console_combobox = new WComboBox();
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
	WContainerWidget *pagetitle_div = new WContainerWidget();
	pagetitle_div->setStyleClass("pagetitle");
	pagetitle_text = new WText();
	pagetitle_div->addWidget(pagetitle_text);

	datacolumn = new WContainerWidget();
	datacolumn->setStyleClass("datacolumn");

	help_text = new WText();
	help_panel = new WPanel();
	help_panel->setTitle(tr("Help"));
	help_panel->setCentralWidget(help_text);

	save_button = new WPushButton(tr("Save"));

	addWidget(pagetitle_div);
	WContainerWidget *ep = new WContainerWidget();
	ep->addWidget(datacolumn);
	ep->addWidget(new WBreak());
	ep->addWidget(save_button);
	ep->addWidget(new WBreak());
	ep->addWidget(help_panel);
	addWidget(ep);
}

//==============================================================================
//===================== BaseServicePage ===========================================
//==============================================================================
BaseServicePage::BaseServicePage(std::string servicename): WContainerWidget()
{
	this->servicename = servicename;

	WContainerWidget *pagetitle_div = new WContainerWidget();
	pagetitle_div->setStyleClass("pagetitle");
	pagetitle_text = new WText();
	pagetitle_div->addWidget(pagetitle_text);

	servicecontrol = new ServiceControlWidget(servicename);

	datacolumn = new WContainerWidget();
	datacolumn->setStyleClass("datacolumn");

	help_text = new WText();
	help_panel = new WPanel();
	help_panel->setTitle(tr("Help"));
	help_panel->setCentralWidget(help_text);

	save_button = new WPushButton(tr("Save"));

	addWidget(pagetitle_div);
	WContainerWidget *ep = new WContainerWidget();
	ep->addWidget(servicecontrol);
	ep->addWidget(datacolumn);
	ep->addWidget(new WBreak());
	ep->addWidget(save_button);
	ep->addWidget(new WBreak());
	ep->addWidget(help_panel);
	addWidget(ep);
}
