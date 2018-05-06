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

using namespace Wt;

//==============================================================================
//===================== ServiceControlWidget =========================================
//==============================================================================
class ServiceControlWidget : public WGroupBox
{
public:
	ServiceControlWidget( std::string service );
	virtual ~ServiceControlWidget(){}
	
	WCheckBox *autostart_checkbox;
private:
	std::string service;

};
//==============================================================================
//===================== BasePage ===========================================
//==============================================================================
class BasePage : public WContainerWidget
{
public:
	BasePage( );
	virtual ~BasePage(){}
protected:
	WText *pagetitle_text;
	WText *help_text;
	WPanel *help_panel;
	WContainerWidget *datacolumn;
	WPushButton *save_button;
};

#endif /* SETTINGSPAGE_H_ */
