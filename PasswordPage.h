/*
 * PasswordPage.h
 *
 *  Created on: 08.04.2012
 *      Author: bond
 */

#ifndef PASSWORDPAGE_H_
#define PASSWORDPAGE_H_

#include "BasePage.h"
#include "Settings.h"
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
using namespace Wt;
//==============================================================================
//===================== PasswordPage ===========================================
//==============================================================================
class PasswordPage : public BasePage
{
public:
	PasswordPage(SettingsDir *sd);
	virtual ~PasswordPage(){}
private:
	void loadParams();
	void saveParams();
private:
	SettingsDir *sd;
	WLineEdit *password_lineedit;
};


#endif /* PASSWORDPAGE_H_ */
