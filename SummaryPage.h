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
#include <Wt/WText>

using namespace Wt;


class SummaryPage: public BasePage
{
public:
	SummaryPage(SettingsDir *sd);

	virtual ~SummaryPage()
	{}

private:
	void updateSummary();
private:
	SettingsDir *sd;
	WText *text_w;
	WText *wifi_state_w;
	WText *eth_state_w;
};

#endif /* SUMMARYPAGE_H_ */
