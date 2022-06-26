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
#include <Wt/WText.h>
#include <Wt/WTable.h>

using namespace Wt;

class NvidiaPage: public BasePage
{
public:
	NvidiaPage(SettingsDir *sd);
	virtual ~NvidiaPage();

	void tableReceived(
			const std::vector<std::map<std::string, std::string> > &gpu_info);

private:

	void saveCoreClock(std::string uuid, WLineEdit *line_edit);
	void saveMemClock(std::string uuid, WLineEdit *line_edit);
	void saveFan(std::string uuid, WLineEdit *line_edit);
	void savePlim(std::string uuid, WLineEdit *line_edit);
	void applyCoreClock(std::string uuid, WLineEdit *line_edit);
	void applyMemClock(std::string uuid, WLineEdit *line_edit);
	void applyFan(std::string uuid, WLineEdit *line_edit);
	void applyPlim(std::string uuid, WLineEdit *line_edit);

private:
	SettingsDir *sd;
	Wt::WTable *table;
};

#endif /* NVIDIAPAGE_H_ */
