/*
 * LogPage.h
 *
 *  Created on: 10.01.2017
 *      Author: bond
 */

#ifndef LOGPAGE_H_
#define LOGPAGE_H_

#include <Wt/WStackedWidget>
#include <Wt/WText>
#include <Wt/WPushButton>
#include <Wt/WTextArea>
#include <Wt/WSocketNotifier>
#include <stdio.h>

using namespace Wt;

class VtState
{
public:
	VtState();
	~VtState();
	bool init(int idx);
	WString grabScreen();

	int rows()
	{
		return vt_rows;
	}

	int cols()
	{
		return vt_cols;
	}

public:
	int idx;
private:
	int vt_fd;
	int vt_cols;
	int vt_rows;
};

//==============================================================================
//===================== VtPage ===========================================
//==============================================================================
class VtPage: public WContainerWidget
{
public:
	VtPage(VtState *vt);

	virtual ~VtPage()
	{}

private:
	void readLog();
private:
	VtState *vt;
	WTextArea *log_textarea;
	WPushButton *refresh_button;
};

#endif /* LOGPAGE_H_ */
