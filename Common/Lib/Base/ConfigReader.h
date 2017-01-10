#pragma once

#include <vector>
#include <string>

struct ConfigElement
{
	wstring configKey;
	wstring configValue;
};

class ConfigReader
{
private:
	vector<ConfigElement> configs;

public:
	bool Open(wstring filename);
	int GetInt(wstring key);
	wstring GetString(wstring key);
};