#include "stdafx.h"
#include "config_manager.h"

property_tree::ptree ConfigManager::_pt;

string ConfigManager::Get(const string& key) {
	return _pt.get<string>(key);
}

int ConfigManager::Get(const string& key, int def) {
	return _pt.get<int>(key);
}

std::map<std::string, std::string> ConfigManager::GetArray(const std::string& key) {
	std::map <std::string, std::string> array;
	
	boost::property_tree::ptree users_array = _pt.get_child(key);
	for (const auto& user_entry : users_array) {
		std::string key = user_entry.first;
		std::string value = user_entry.second.data();
		array[key] = value;
	}

	return array;
}

bool ConfigManager::Load(const char* file) {
	try {

		ifstream ifs(file);

		property_tree::read_json(ifs, _pt);

		ifs.close();
	}
	catch (const std::exception& ex) {
		return false;
	}

	return true;
}

