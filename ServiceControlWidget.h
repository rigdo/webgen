/*
 * BasePage.h
 *
 *  Created on: 03.04.2012
 *      Author: bond
 */

#ifndef SERVICECONTROLWIDGET_H_
#define SERVICECONTROLWIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WCheckBox>
#include <Wt/WPushButton>
#include <Wt/WGroupBox>

using namespace Wt;

//==============================================================================
//===================== ServiceControlWidget =========================================
//==============================================================================
class ServiceControlWidget: public WGroupBox
{
public:
	ServiceControlWidget(std::string service);

	virtual ~ServiceControlWidget()
	{}

	void start();
	void stop();
	void restart();
	std::string getState();

private:
	void updateView();
	void addService();
	void delService();

private:
	WText *startstopstate;
	WPushButton *start_button;
	WPushButton *stop_button;
	WPushButton *restart_tbutton;
	std::string service;
};

#endif /* SERVICECONTROLWIDGET_H_ */
