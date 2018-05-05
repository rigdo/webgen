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
class Ethminer : public BasePage
{
public:
	Ethminer(SettingsDir *sd );
	virtual ~Ethminer(){}
	
private:
	void loadParams();
	void saveParams();
	void modeChanged();

private:
	SettingsDir *sd;
	WCheckBox *autostart_checkbox;
	WLineEdit *pool_lineedit;
	WLineEdit *user_lineedit;
	WComboBox *console_combobox;
};

#endif /* ETHMINER_H_ */
