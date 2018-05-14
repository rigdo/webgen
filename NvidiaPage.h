/*
 * NvidiaPage.h
 *
 *  Created on: 13.05.2018
 *      Author: bond
 */

#ifndef NVIDIAPAGE_H_
#define NVIDIAPAGE_H_

#include "BasePage.h"
#include "Settings.h"
#include <Wt/WText>
#include <Wt/WTable>
#include "nvml.h"

using namespace Wt;

class NvidiaPage: public BasePage
{
public:
	NvidiaPage(SettingsDir *sd);
	virtual ~NvidiaPage();
private:
	Wt::WTable *table;
};

#endif /* NVIDIAPAGE_H_ */
