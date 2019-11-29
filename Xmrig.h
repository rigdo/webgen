/*
 * Xmrig.h
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#ifndef XMRIGPAGE_H_
#define XMRIGPAGE_H_

#include "BasePage.h"
#include "Settings.h"
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WComboBox>
#include <Wt/WCheckBox>

using namespace Wt;

//==============================================================================
//===================== Xmrig ===========================================
//==============================================================================
class Xmrig: public BasePage
{
public:
	Xmrig(SettingsDir *sd);

	virtual ~Xmrig()
	{}

private:
	void loadParams();
	void saveParams();
	void modeChanged();

private:
	SettingsDir *sd;
	WCheckBox *autostart_checkbox;
	WLineEdit *pool_lineedit;
	WLineEdit *user_lineedit;
	WLineEdit *pass_lineedit;
	WComboBox *algo_combobox;
	WComboBox *nicehash_combobox;
	WComboBox *console_combobox;
};

#endif /* XMRIGPAGE_H_ */
