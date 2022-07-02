/*
 * RebootPage.cpp
 *
 *  Created on: 04.06.2012
 *      Author: bond
 */

#include "RebootPage.h"

//==============================================================================
//===================== LogPage ===========================================
//==============================================================================
RebootPage::RebootPage( ) : BasePage()
{
	pagetitle_text->setText(tr("Reboot"));
	help_text->setText( WString(tr("reboot_page_help")) );

	{
		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("buttonscontainer");
		reboot_button = c->addWidget(std::make_unique<WPushButton>(tr("Reboot")));
	}
	save_button->hide();
	reboot_button->clicked().connect( this, &RebootPage::rebootPressed );
}

RebootPage::~RebootPage() 
{
}

void RebootPage::rebootPressed()
{
	system("/sbin/reboot");
}
