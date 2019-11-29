/*
 * BasePage.h
 *
 *  Created on: 03.04.2012
 *      Author: bond
 */

#ifndef SETTINGSPAGE_H_
#define SETTINGSPAGE_H_

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WCheckBox>
#include <Wt/WPushButton>
#include <Wt/WGroupBox>
#include <Wt/WPanel>
#include <Wt/WComboBox>

#include "ServiceControlWidget.h"

using namespace Wt;

WComboBox *buildConsoleComboBox();

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
