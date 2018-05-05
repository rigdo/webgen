/*
 * BasePage.cpp
 *
 *  Created on: 03.04.2012
 *      Author: bond
 */
#include "BasePage.h"
#include <Wt/WPanel>
#include <Wt/WBreak>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


//==============================================================================
//===================== ServiceControlWidget =========================================
//==============================================================================
ServiceControlWidget::ServiceControlWidget( std::string service )
{
	this->service = service;
	setTitle( tr("Service Contol"));
	autostart_checkbox = new WCheckBox();

	WContainerWidget *buttonscontainer = new WContainerWidget();
	buttonscontainer->setStyleClass("servicebuttonscontainer");
//	addWidget( buttonscontainer );
	
	{
	WContainerWidget *c = new WContainerWidget();
	c->setStyleClass("setting");
	WText *label = new WText(tr("Control"));
	label->setStyleClass("shortlabel");
	c->addWidget( label );
	c->addWidget( buttonscontainer );
	addWidget( c );
	}
	{
	WContainerWidget *c = new WContainerWidget();
	c->setStyleClass("setting");
	WText *label = new WText(tr("State"));
	label->setStyleClass("shortlabel");
	c->addWidget( label );
//	c->addWidget( startstopstate );
	addWidget( c );
	}
	{
	WContainerWidget *c = new WContainerWidget();
	c->setStyleClass("setting");
	WText *label = new WText(tr("Autostart"));
	label->setStyleClass("shortlabel");
	c->addWidget( label );
	c->addWidget( autostart_checkbox );
	addWidget( c );
	}
}


//==============================================================================
//===================== BasePage ===========================================
//==============================================================================
BasePage::BasePage() : WContainerWidget()
{
	help_text = new WText( );
	
	WContainerWidget *label_div = new WContainerWidget();
	label_div->setStyleClass("pagetitle");
	pagetitle_text = new WText();
	label_div->addWidget( pagetitle_text );
	addWidget( label_div);
	WContainerWidget *ep = new WContainerWidget();
	{
		datacolumn = new WContainerWidget();
		datacolumn->setStyleClass("datacolumn");
		WContainerWidget *helpcolumn = new WContainerWidget();
		helpcolumn->setStyleClass("helpcolumn");
		
		WPanel *help_panel = new WPanel();
		help_panel->setTitle(tr("Help"));
		help_panel->setCentralWidget( help_text );
		helpcolumn->addWidget(help_panel);

		ep->addWidget( datacolumn );
		ep->addWidget( helpcolumn );

	}
	addWidget( ep );
}
