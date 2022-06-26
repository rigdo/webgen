/*
 * LocalGui.h
 *
 *  Created on: 08.05.2018
 *      Author: bond
 */

#ifndef LOCALGUI_H_
#define LOCALGUI_H_

#include <string>

#include <Wt/WContainerWidget.h>

using namespace Wt;

//==============================================================================
//===================== SiteGui ================================================
//==============================================================================
class LocalGui: public WContainerWidget
{
public:
	LocalGui(std::string settings_dir);

	virtual ~LocalGui()
	{}

private:
	void setLangEn();
	void setLangRu();
private:
};

#endif /* LOCALGUI_H_ */
