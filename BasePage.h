/*
 * BasePage.h
 *
 *  Created on: 03.04.2012
 *      Author: bond
 */

#ifndef SETTINGSPAGE_H_
#define SETTINGSPAGE_H_

#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WCheckBox.h>
#include <Wt/WPushButton.h>
#include <Wt/WGroupBox.h>
#include <Wt/WPanel.h>
#include <Wt/WComboBox.h>

#include "ServiceControlWidget.h"

using namespace Wt;

WComboBox *buildConsoleComboBox(WContainerWidget *father);

//==============================================================================
//===================== BasePage ===========================================
//==============================================================================
class BasePage: public WContainerWidget
{
public:
	BasePage();

	virtual ~BasePage()
	{}

protected:
	WText *pagetitle_text;
	WText *help_text;
	WPanel *help_panel;
	WContainerWidget *datacolumn;
	WPushButton *save_button;
};

//==============================================================================
//===================== BasePage ===========================================
//==============================================================================
class BaseServicePage: public WContainerWidget
{
public:
	BaseServicePage(std::string servicename);

	virtual ~BaseServicePage()
	{}

protected:
	std::string servicename;
	WText *pagetitle_text;
	WText *help_text;
	WPanel *help_panel;
	WContainerWidget *datacolumn;
	WPushButton *save_button;
	ServiceControlWidget *servicecontrol;
};
#endif /* SETTINGSPAGE_H_ */
