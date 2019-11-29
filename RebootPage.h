/*
 * RebootPage.h
 *
 *  Created on: 04.06.2012
 *      Author: bond
 */

#ifndef REBOOTPAGE_H_
#define REBOOTPAGE_H_
#include "BasePage.h"
//==============================================================================
//===================== RebootPage ===========================================
//==============================================================================

class RebootPage : public BasePage
{
public:
	RebootPage();
	virtual ~RebootPage();
	
private:
	void rebootPressed();

private:
	WPushButton *reboot_button;
};

#endif /* REBOOTPAGE_H_ */

