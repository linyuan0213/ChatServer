#ifndef __CONFIG_H
#define __CONFIG_H
#include <string>
#include <map>

int load_config(const char* config_path, std::map<std::string, std::string> &map);

#endif 
