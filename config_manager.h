#pragma once

#include <map>

using namespace std;
using namespace boost;

class ConfigManager
{
public:
	static bool Load(const char* file_name);
	static string Get(const string& key);
	static int Get(const string& key, int def);
	static std::map<std::string, std::string> GetArray(const std::string& key);
private:
	static property_tree::ptree _pt;
};