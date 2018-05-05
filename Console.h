/*
 * Console.h
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#ifndef CONSOLEPAGE_H_
#define CONSOLEPAGE_H_

#include "BasePage.h"
#include "Settings.h"
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WComboBox>
using namespace Wt;

//==============================================================================
//===================== Console ===========================================
//==============================================================================
class Console : public BasePage
{
public:
	Console( SettingsDir *sd);
	virtual ~Console(){}
	
private:
	void loadParams();
	void saveParams();
	void modeChanged();

private:
	SettingsDir *sd;
	WComboBox *owner_combobox[6];
	WLineEdit *pool_lineedit;
	WLineEdit *user_lineedit;
};

#endif /* CONSOLEPAGE_H_ */
