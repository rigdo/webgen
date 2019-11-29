/*
 * WiFiPage.h
 *
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
class WiFiPage: public BasePage
{
public:
	WiFiPage(SettingsDir *sd);

	virtual ~WiFiPage()
	{}

private:
	void loadParams();
	void saveParams();

private:
	SettingsDir *sd;
	WCheckBox *autostart_checkbox;
	WLineEdit *ssid_lineedit;
	WLineEdit *psk_lineedit;
};

#endif /* WIFIPAGE_H_ */
