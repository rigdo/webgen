#ifndef WEBGUI_H_
#define WEBGUI_H_

#include "Settings.h"
#include "EthernetPage.h"
#include "OpenVpnPage.h"
#include "WiFiPage.h"
#include "Xmrig.h"
#include "XmrigNvidia.h"
#include "XmrigAmd.h"
#include "Ethminer.h"

#include <vector>
#include <string>

#include <Wt/WContainerWidget>
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
	void handleRequest(const Wt::Http::Request& request,
			Wt::Http::Response& response);
private:
	std::string settings_dir;
};

//==============================================================================
//===================== SiteGui ================================================ 
//==============================================================================
class SiteGui : public WContainerWidget
{
public:
	SiteGui(std::string settings_dir);
	virtual ~SiteGui( ){}
private:
};

#endif /* WEBGUI_H_ */
