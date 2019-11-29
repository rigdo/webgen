/*
 * XmrigAmd.h
 */

#ifndef XMRIGAPAGE_H_
#define XMRIGAPAGE_H_

#include "BasePage.h"
#include "Settings.h"
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WComboBox>
#include <Wt/WCheckBox>

using namespace Wt;

//==============================================================================
//===================== XmrigAmd ===========================================
//==============================================================================
class XmrigAmd: public BasePage
{
public:
	XmrigAmd(SettingsDir *sd);

	virtual ~XmrigAmd()
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
	WLineEdit *opencl_launch_lineedit;
	WComboBox *algo_combobox;
	WComboBox *nicehash_combobox;
	WComboBox *console_combobox;
};

#endif /* XMRIGAPAGE_H_ */
