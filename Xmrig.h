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
class Xmrig: public BaseServicePage
{
public:
	Xmrig(SettingsDir *sd, std::string gpu_vendor, std::string servicename);

	virtual ~Xmrig()
	{}

private:
	void loadParams();
	void saveParams();
	void modeChanged();

private:
	SettingsDir *sd;
	std::string gpu_vendor;
	WCheckBox *autostart_checkbox;
	WLineEdit *pool_lineedit;
	WLineEdit *user_lineedit;
	WLineEdit *pass_lineedit;
	WLineEdit *donate_lineedit;
	WComboBox *algo_combobox;
	WComboBox *nicehash_combobox;
	WComboBox *console_combobox;

	WLineEdit *opencl_launch_lineedit;
	WLineEdit *cuda_launch_lineedit;
};

#endif /* XMRIGPAGE_H_ */
