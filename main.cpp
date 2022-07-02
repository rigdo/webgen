#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>
#include <Wt/WServer.h>
#include "SiteGui.h"
#include "Settings.h"
#include <sys/stat.h>
#include <sys/types.h>

std::string generateSettingsDir()
{
	char buf[256];
	strcpy(buf, "./store_XXXXXX");
	mkdtemp(buf);
	std::string store_dir(buf);
	std::string sdir = store_dir + "/settings";
	mkdir(sdir.c_str(), 0777);
	return sdir;
}

std::unique_ptr<WApplication> createApplication(const WEnvironment &env)
{
	std::unique_ptr<WApplication> app = std::make_unique<Wt::WApplication>(env);
	app->setCssTheme("polished");
	app->messageResourceBundle().use(app->docRoot() + "/webgui");

	std::string sdir = generateSettingsDir();
	app->root()->addWidget(std::make_unique<SiteGui>(sdir));
	app->setTitle("rigdo settings generator");
	app->useStyleSheet("css/style.css");
	return app;
}

int main(int argc, char *argv[])
{
	try {
		WServer server(argv[0]);
		server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
		server.addEntryPoint(Wt::EntryPointType::Application, createApplication);
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
