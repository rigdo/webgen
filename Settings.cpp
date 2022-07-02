/*
 * Settings.cpp
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */
#include "Settings.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
//==============================================================================
//===================== Settings ===========================================
//==============================================================================
// /tmp/settings/$service$instance
// /tmp/settings/kernel
// /tmp/settings/services
// /tmp/settings/userspace

std::map<std::string, std::map<WString, WString> > Settings::gcache;

Settings::Settings(std::string dir, std::string service, int instance,
		bool ignore_cache)
{
	this->dir = dir;
	this->service = toOneName(service, instance);
	loadVars(ignore_cache);
}

// thans to Daniel T.
// http://groups.google.com/group/comp.lang.c++/msg/f0985ee0d82129bb
const std::string whiteSpaces(" \f\n\r\t\v");
void trimRight(std::string &str, const std::string &trimChars = whiteSpaces);
void trimLeft(std::string &str, const std::string &trimChars = whiteSpaces);
void trim(std::string &str, const std::string &trimChars = whiteSpaces);

void trimRight(std::string &str, const std::string &trimChars)
{
	std::string::size_type pos = str.find_last_not_of(trimChars);
	str.erase(pos + 1);
}

void trimLeft(std::string &str, const std::string &trimChars)
{
	std::string::size_type pos = str.find_first_not_of(trimChars);
	str.erase(0, pos);
}

void trim(std::string &str, const std::string &trimChars)
{
	trimRight(str, trimChars);
	trimLeft(str, trimChars);
}

bool Settings::loadVars(bool ignore_cache)
{
	printf("loadVars %s\n", service.c_str());
	if (!ignore_cache) {
		if (gcache.find(service) != gcache.end()) {
			values = gcache[service];
			return true;
		} else {
			printf("cache miss for %s\n", service.c_str());
		}
	}

	const char *script_loadvars = "/usr/bin/dumpvars.sh";
	if (getenv("WEBGUI_DUMPVARS"))
		script_loadvars = getenv("WEBGUI_DUMPVARS");

	char cmd[256];
	cmd[255] = 0;
	snprintf(cmd, 255, "%s %s %s", script_loadvars, dir.c_str(),
			service.c_str());
	FILE *fp = popen(cmd, "r");
	if (!fp)
		return false;
	char buf[4096];
	while (!feof(fp)) {
		buf[0] = 0;
		buf[sizeof(buf) - 1] = 0;
		fgets(buf, sizeof(buf) - 1, fp);
		std::string line(buf);
		size_t pos = line.find('=');
		if (pos == std::string::npos)
			continue;
		std::string name = line.substr(0, pos);
		trim(name);
		std::string value = line.substr(pos + 1, std::string::npos);
		trim(value);
		trim(value, "'");
		trim(value, "\"");
		values[name] = value;
//		printf("\"%s\" = \"%s\"\n", name.c_str(), value.c_str() );
	}
	pclose(fp);
	gcache[service] = values;
	return true;
}

void Settings::dump()
{
	for (std::map<WString, WString>::iterator it = values.begin();
			it != values.end(); it++) {
		WString name = it->first;
		WString value = it->second;
		printf("\"%s\" = \"%s\"\n", name.toUTF8().c_str(),
				value.toUTF8().c_str());
	}
}

bool Settings::saveVar(WString name, WString value)
{
	const char *script_savevar = "/usr/bin/savevar.sh";
	if (getenv("WEBGUI_SAVEVAR"))
		script_savevar = getenv("WEBGUI_SAVEVAR");

	char cmd[1024];
	cmd[1023] = 0;
	snprintf(cmd, 1023, "%s %s '%s' '%s' '%s'", script_savevar, dir.c_str(),
			service.c_str(), name.toUTF8().c_str(), value.toUTF8().c_str());
	printf("run : '%s'\n", cmd);
	// big  security hole! -> ToDo fork and exec
	int ret = system(cmd);
	return (ret == 0);
}

WString Settings::value(WString name, WString defval)
{
	if (values.find(name) == values.end())
		return defval;
	return values[name];
}

bool Settings::save(WString name, WString value)
{
	if ((values.find(name) != values.end()) && (values[name] == value))
		return true;
	bool ret = saveVar(name, value);
	if (ret) {
		values[name] = value;
		gcache[service][name] = value;
	}
	return ret;
}

WString Settings::additionalSettingsFileName(WString postfix)
{
	WString dir = "/tmp/settings";
	if (getenv("WEBGUI_SETTNGSDIR"))
		dir = getenv("WEBGUI_SETTNGSDIR");
	return dir + "/" + service + "_" + postfix;
}

WString Settings::additionalSettingsDirName(WString postfix)
{
	return additionalSettingsFileName(postfix);
}

//-------
std::string Settings::toOneName(std::string service, int instance)
{
	if (instance == -1) {
		return service;
	}
	char buf[256];
	sprintf(buf, "%s%d", service.c_str(), instance);
	return std::string(buf);
}

std::string Settings::loadFile(const char *fname, int maxsize)
{
	FILE *fp = fopen(fname, "rt");
	if (!fp)
		return std::string();
	char *buf = new char[maxsize];
	int readed = 0;
	while (!feof(fp)) {
		int ret = fread(buf + readed, 1, maxsize - readed, fp);
		if (ret < 0) {
			printf("error read %s : %s\n", fname, strerror(errno));
			delete[] buf;
			fclose(fp);
			return std::string();
		}
		if (ret == 0)
			break;
		readed += ret;
		if (readed >= maxsize) {
			printf("too big %s\n", fname);
			delete[] buf;
			fclose(fp);
			return std::string();
		}
	}
	std::string ret(buf, readed);
	delete[] buf;
	fclose(fp);
	return ret;
}

bool Settings::saveFile(const char *fname, std::string data)
{
	FILE *fp = fopen(fname, "wt");
	if (!fp)
		return false;
	int ret = fwrite(data.c_str(), data.length(), 1, fp);
	if (ret != 1) {
		printf("can't write openvpn static.key(%s) : %s\n", fname,
				strerror(errno));
		return false;
	}
	fclose(fp);
	return true;
}

//==============================================================================
//===================== Settings ===========================================
//==============================================================================
SettingsDir::SettingsDir(const std::string &dir)
{
	this->dir = dir;
}

Settings
SettingsDir::byService(std::string service, int instance, bool ignore_cache)
{
	return Settings(dir, service, instance, ignore_cache);
}
