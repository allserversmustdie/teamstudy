#include "Precompiled.h"
#include "ConfigReader.h"
#include "StringUtil.h"
#include <fstream>
#include <iostream>

using namespace std;

bool ConfigReader::Open(wstring filename)
{
	wstring inputLine;
	wifstream configFile(filename.c_str());
	if (!configFile.is_open())
		return false;

	while (configFile.good()) 
	{
		getline(configFile, inputLine);
		ConfigElement newConfig;
			
		size_t equalsSign = inputLine.find(L"=");
			
		if (equalsSign != string::npos)
		{
			newConfig.configKey = trim(inputLine.substr(0, equalsSign));
			newConfig.configValue = trim(inputLine.substr(equalsSign + 1));
			configs.push_back(newConfig);
		}
	}
		
	configFile.close();
	return true;
}

int ConfigReader::GetInt(wstring key)
{
	return _wtoi(GetString(key).c_str());
}

wstring ConfigReader::GetString(wstring key)
{
	for (int i = 0; i < configs.size(); i++) 
	{
		if (configs[i].configKey == key)
		{
			return configs[i].configValue;
		}
	}

	return L"";
}