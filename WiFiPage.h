/*
 * WiFiPage.h
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#ifndef WIFIPAGE_H_
#define WIFIPAGE_H_

#include "BasePage.h"
#include "Settings.h"
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WComboBox>
#include <Wt/WCheckBox>
using namespace Wt;

//==============================================================================
//===================== WiFiPage ===========================================
//==============================================================================
class WiFiPage : public BasePage
{
public:
	WiFiPage( SettingsDir *sd );
	virtual ~WiFiPage(){}
	
private:
	void loadParams();
	void saveParams();
	void modeChanged();

private:
	SettingsDir *sd;
	WCheckBox *autostart_checkbox;
	WLineEdit *ssid_lineedit;
	WLineEdit *psk_lineedit;
	
	WPushButton *save_button;
};

#endif /* WIFIPAGE_H_ */