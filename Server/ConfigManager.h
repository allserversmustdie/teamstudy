#pragma once

#include "GlobalDefines.h"

struct ServerConfig
{
	wstring ip;
	unsigned short port;

	ServerConfig()
		: ip(L""), port(0)
	{};
};

struct DBConfig
{
	wstring ip;
	unsigned short port;
	wstring user;
	wstring passwd;
	wstring dbname;

	DBConfig()
		: ip(L""), port(0), user(L""), passwd(L""), dbname(L"")
	{};
};

class ConfigManager
{
public:
	ConfigManager();

	ServerConfig serverConfig;
	DBConfig dbConfig[MAX_DB_HANDLE];
};
