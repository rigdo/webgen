/*
 * SummaryPage.h
 *
 *  Created on: 08.05.2018
 *      Author: bond
 */

#ifndef SUMMARYPAGE_H_
#define SUMMARYPAGE_H_

#include "BasePage.h"
#include "Settings.h"
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WComboBox>
#include <Wt/WCheckBox>

using namespace Wt;


class SummaryPage : public BasePage
{
public:
	SummaryPage(SettingsDir *sd);
	virtual ~SummaryPage(){}

private:
	SettingsDir *sd;

};

#endif /* SUMMARYPAGE_H_ */
