/*
 * Ethminer.h
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#ifndef ETHMINER_H_
#define ETHMINER_H_

#include "BasePage.h"
#include "Settings.h"
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WComboBox>
#include <Wt/WCheckBox>

using namespace Wt;

//==============================================================================
//===================== Ethminer ===========================================
//==============================================================================
class Ethminer: public BaseServicePage
{
public:
	Ethminer(SettingsDir *sd, std::string gpu_vendor, std::string servicename);

	virtual ~Ethminer()
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
	WLineEdit *worker_lineedit;
	WComboBox *scheme_combobox;
	WComboBox *console_combobox;
	WComboBox *opencl_mode_combobox;
};

#endif /* ETHMINER_H_ */
