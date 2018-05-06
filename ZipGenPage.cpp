/*
 * ZipGenPage.cpp
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#include "ZipGenPage.h"
#include <Wt/Http/Response>


//==============================================================================
//===================== ZipGen ================================================= 
//==============================================================================
ZipGen::ZipGen(std::string settings_dir)
{
	this->settings_dir = settings_dir;
	suggestFileName("rigdo_settings.zip");
}

ZipGen::~ZipGen()
{
	beingDeleted(); // see "Concurrency issues" below.
}

void ZipGen::handleRequest(const Wt::Http::Request& request,
		Wt::Http::Response& response)
{
	response.setMimeType("application/zip");
		
	char cmd[ 1024 ];
	snprintf(cmd, 1023, "cd %s/.. && zip -r - settings", settings_dir.c_str());
	printf("run : '%s'\n", cmd);

	FILE *fp = popen(cmd, "r");
	if (!fp) {
		printf("can't do popen for cmd: \"%s\"\n", cmd);
		response.setStatus(500);
		return;
	}
	int maxsize = 100 * 1024;
	char *buf = new char[maxsize];
	while (!feof(fp)) {
		int ret = fread(buf, 1, maxsize, fp);
		if (ret <= 0)
			break;
		response.out().write(buf, ret);
	}
	delete[] buf;

	int retcode = pclose(fp);
	if (retcode != 0)
		response.setStatus(500);
}

//==============================================================================
//===================== ZipGenPage ===========================================
//==============================================================================
ZipGenPage::ZipGenPage(std::string settings_dir) : BasePage()
{
	pagetitle_text->setText(tr("zipgen_title"));
	help_text->setText( tr("zipgen_help"));
		
	ZipGen *zip_gen = new ZipGen(settings_dir);

	zip_button = new WPushButton(tr("download_zip"));
	{
	WContainerWidget *c = new WContainerWidget();
	c->setStyleClass("setting");
	WText *label = new WText(tr("Result"));
	label->setStyleClass("label");
	c->addWidget( label );
	c->addWidget( zip_button );
	datacolumn->addWidget( c );
	}
	
	save_button->hide();
	help_panel->hide();
	
	zip_button->setLink(WLink(zip_gen->url()));
}

