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
	reboot_button = new WPushButton(tr("Reboot"));
	{
		WContainerWidget *buttonscontainer = new WContainerWidget();
		buttonscontainer->setStyleClass("buttonscontainer");
		buttonscontainer->addWidget( reboot_button );
		datacolumn->addWidget( buttonscontainer );
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
