/*
 * EthernetPage.h
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#ifndef ETHERNETPAGE_H_
#define ETHERNETPAGE_H_

#include "BasePage.h"
#include "Settings.h"

#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WComboBox.h>

using namespace Wt;

//==============================================================================
//===================== EthernetPage ===========================================
//==============================================================================
class EthernetPage: public BasePage
{
public:
	EthernetPage(SettingsDir *sd);

	virtual ~EthernetPage()
	{}

private:
	void loadParams();
	void saveParams();
	void modeChanged();

private:
	SettingsDir *sd;
	WComboBox *mode_combobox;

	//Static net
	WContainerWidget *static_mode_conteiner;
	WLineEdit *ip_lineedit;
	WLineEdit *mask_lineedit;
	WLineEdit *gateway_lineedit;
	WLineEdit *dns_lineedit;
};

#endif /* ETHERNETPAGE_H_ */
