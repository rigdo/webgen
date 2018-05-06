#ifndef WEBGUI_H_
#define WEBGUI_H_

#include <string>

#include <Wt/WContainerWidget>

using namespace Wt;

//==============================================================================
//===================== SiteGui ================================================ 
//==============================================================================
class SiteGui : public WContainerWidget
{
public:
	SiteGui(std::string settings_dir);
	virtual ~SiteGui( ){}
private:	
	void setLangEn();
	void setLangRu();
private:
};

#endif /* WEBGUI_H_ */
