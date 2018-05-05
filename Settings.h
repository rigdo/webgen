/*
 * Settings.h
 *
 *  Created on: 04.04.2012
 *      Author: bond
 */

#ifndef SETINGS_H_
#define SETINGS_H_
#include <Wt/WString>
#include <map>
#include <list>
#include <stdlib.h>


using namespace Wt;
//==============================================================================
//===================== Settings ===========================================
//==============================================================================
class Settings
{
public:
	Settings( std::string dir, std::string service, int instance = -1, 
			bool ignore_cache = false );

	WString value( WString name, WString defval );
	WString value( WString name, int idx, WString defval )
	{
		WString n = (name + "_{1}").arg(idx);
		return value( n, defval );
	}
	int valueInt( WString name, int defval )
	{
		WString str = value( name, WString("{1}").arg(defval));
		return strtol(str.toUTF8().c_str(), 0, 0);
	}
	int valueInt( WString name, int idx, int defval )
	{
		WString str = value( name, idx, WString("{1}").arg(defval));
		return strtol(str.toUTF8().c_str(), 0, 0);
	}

	bool save( WString name, WString value );
	bool save( WString name, int idx, WString value )
	{
		WString n = (name + "_{1}").arg(idx);
		return save( n, value );
	}
	bool saveInt( WString name, int value )
	{
		return save( name, WString("{1}").arg(value));
	}
	bool saveInt( WString name, int idx, int value )
	{
		return save( name, idx, WString("{1}").arg(value));
	}
	
	WString additionalSettingsFileName( WString postfix );
	WString additionalSettingsDirName( WString postfix );
	void dump();

private:
	bool loadVars( bool ignore_cache = false );
	bool saveVar( WString name, WString value );
	
public:
	static std::string toOneName( std::string service, int instance );
	
	static std::string loadFile( const char *fname, int maxsize );
	static bool saveFile( const char *fname, std::string data );

private:
	std::map<WString,WString> values;
	std::string service;
	std::string dir;
	
private:
	static std::map< std::string, std::map<WString,WString> > gcache;
};

//==============================================================================
//===================== Settings ===========================================
//==============================================================================
class SettingsDir
{
public:
	SettingsDir( std::string dir);
	Settings byService(std::string service, int instance = -1, 
			bool ignore_cache = true);
	
private:
	std::string dir;
};

#endif /* SETINGS_H_ */
