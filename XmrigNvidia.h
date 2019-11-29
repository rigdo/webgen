/*
 * XmrigNvidia.h
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#ifndef XMRIGNPAGE_H_
#define XMRIGNPAGE_H_

#include "BasePage.h"
#include "Settings.h"
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WComboBox>
#include <Wt/WCheckBox>

using namespace Wt;

//==============================================================================
//===================== XmrigNvidia ===========================================
//==============================================================================
class XmrigNvidia: public BasePage
{
public:
	XmrigNvidia(SettingsDir *sd);

	virtual ~XmrigNvidia()
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
	WLineEdit *cuda_launch_lineedit;
	WComboBox *algo_combobox;
	WComboBox *nicehash_combobox;
	WComboBox *console_combobox;
};

#endif /* XMRIGNPAGE_H_ */
