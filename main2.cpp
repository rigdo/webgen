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

std::string readVersion()
{
	FILE *fp = fopen("/etc/version", "rt");
	if (!fp) {
		perror("can't read /etc/version : ");
		return "";
	}
	std::string version;
	char buf[4096];
	while (!feof(fp)) {
		buf[0] = 0;
		buf[sizeof(buf) - 1] = 0;
		fgets(buf, sizeof(buf) - 1, fp);
		std::string line(buf);
		std::string::size_type nlnpos = line.find_last_not_of("\n");
		line.erase(nlnpos + 1);

		size_t pos = line.find('=');
		if (pos == std::string::npos)
			continue;
		std::string name = line.substr(0, pos);
		std::string value = line.substr(pos + 1, std::string::npos);
		if (name == "VERSION") {
			version = value;
			break;
		}
//      printf("\"%s\" = \"%s\"\n", name.c_str(), value.c_str() );
	}
	fclose(fp);
	return version;
}

WApplication *createApplication(const WEnvironment &env)
{
	WApplication *app = new WApplication(env);
	app->setCssTheme("polished");
	app->messageResourceBundle().use(app->docRoot() + "/webgui");

	std::string sdir = "";
	app->root()->addWidget(new LocalGui(sdir));
	std::string version = readVersion();
	app->setTitle(WString::tr("rigdo control pannel").arg(version));
	app->useStyleSheet("css/style.css");
	return app;
}

int main(int argc, char *argv[])
{
	try {
		WServer server(argv[0]);
		server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
		server.addEntryPoint(Wt::Application, createApplication);
		if (server.start()) {
			WServer::waitForShutdown();
			server.stop();
		}
	} catch (WServer::Exception &e) {
		std::cerr << e.what() << std::endl;
	} catch (std::exception &e) {
		std::cerr << "exception: " << e.what() << std::endl;
	}
}
