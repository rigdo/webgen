/*
 * ZipGenPage.h
 */

#ifndef ZIPGENPAGE_H_
#define ZIPGENPAGE_H_

#include "BasePage.h"
#include <Wt/WPushButton>
#include <Wt/WResource>

using namespace Wt;

//==============================================================================
//===================== ZipGen ================================================= 
//==============================================================================
class ZipGen: public Wt::WResource
{
public:
	ZipGen(std::string settings_dir);
	~ZipGen();
	void handleRequest(const Wt::Http::Request &request,
			Wt::Http::Response &response);
private:
	std::string settings_dir;
};

//==============================================================================
//===================== ZipGenPage ===========================================
//==============================================================================
class ZipGenPage: public BasePage
{
public:
	ZipGenPage(std::string settings_dir);

	virtual ~ZipGenPage()
	{}

private:
	void loadParams();
	void saveParams();

private:
	WPushButton *zip_button;
};

#endif /* ZIPGENPAGE_H_ */
