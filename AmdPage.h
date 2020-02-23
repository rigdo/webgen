/*
 * AmdPage.h
 *
 *  Created on: 22.02.2020
 *      Author: bond
 */

#ifndef AMDPAGE_H_
#define AMDPAGE_H_

#include "BasePage.h"
#include "Settings.h"
#include <Wt/WText>
#include <Wt/WTable>

using namespace Wt;

class AmdPage: public BasePage
{
public:
	AmdPage(SettingsDir *sd);
	virtual ~AmdPage();

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

	bool device_by_uuid(std::string uuid, uint32_t *device);

private:
	SettingsDir *sd;
	Wt::WTable *table;
	
	std::vector<std::map<std::string, std::string> > gpu_info;
};

#endif /* AMDPAGE_H_ */
