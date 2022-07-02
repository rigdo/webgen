/*
 * PasswordPage.cpp
 *
 *  Created on: 08.04.2012
 *      Author: bond
 */

#include "PasswordPage.h"
#include "Settings.h"
//#define _XOPEN_SOURCE
#include <unistd.h>
#include <errno.h>

//==============================================================================
//===================== DIPortsPage ===========================================
//==============================================================================
PasswordPage::PasswordPage(SettingsDir *sd) : BasePage()
{
	this->sd = sd;
	pagetitle_text->setText(tr("Root password"));
	help_text->setText( WString(tr("password_page_help")) );

	{
		WContainerWidget *c = datacolumn->addWidget(std::make_unique<WContainerWidget>());
		c->setStyleClass("setting");
		WText *label = c->addWidget(std::make_unique<WText>(tr("New root password")));
		label->setStyleClass("label");
		password_lineedit = c->addWidget(std::make_unique<WLineEdit>());
		password_lineedit->setTextSize(49);
		password_lineedit->setStyleClass("settingvalue");
	}
	save_button->clicked().connect( this, &PasswordPage::saveParams );
	loadParams();
}

void PasswordPage::loadParams()
{
//	Settings s( "userspace" );
//	password_lineedit->setText( s.value("LOGIN_ROOTPASS","root") );
}

void PasswordPage::saveParams()
{
//	Wt::Auth::User rootuser( "root", session->usersDbLink() );
//	Session::passwordAuth().updatePassword( rootuser,
//			password_lineedit->text() );
//	Wt::Auth::PasswordHash hash = session->usersDbLink().password( rootuser );
	const char *salt = "$5$QEWL6sO.qykYh8Jo"; // sha256
	const char *e = crypt(password_lineedit->text().toUTF8().c_str() ,salt);
	if(!e) {
		printf("err %d\n", errno);
		return;
	}
	WString val = WString::fromUTF8(e);

	Settings s = sd->byService("userspace");
	s.save("LOGIN_ROOTPASS", val );
}
