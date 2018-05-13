/*
 * localgui.cpp
 *
 *  Created on: 08.05.2018
 *      Author: bond
 */


#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WServer>
#include "LocalGui.h"
#include "Settings.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <Wt/WString>
using namespace Wt;                                                             


WApplication *createApplication(const WEnvironment& env)
{
	WApplication* app = new WApplication(env);
	app->setCssTheme("polished");
	app->messageResourceBundle().use(app->docRoot() + "/webgui");

//	std::string sdir = "/mnt/store/settings";
	std::string sdir = "settings";

	app->root()->addWidget( new LocalGui(sdir) );
	app->setTitle("rigdo control pannel");
	app->useStyleSheet("css/style.css");
//	app->addMetaHeader(MetaHttpHeader,"refresh", "1");
	return app;
}

int main( int argc, char *argv[] )
{
	try {
		WServer server(argv[0]);
		server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
		server.addEntryPoint(Wt::Application, createApplication);
		if (server.start()) {
			WServer::waitForShutdown();
			server.stop();
		}
	} catch (WServer::Exception& e) {
		std::cerr << e.what() << std::endl;
	} catch (std::exception &e) {
		std::cerr << "exception: " << e.what() << std::endl;
	}
}
