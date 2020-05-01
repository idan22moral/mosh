#ifndef _MOSH_ENVIRONMENT_H
#define _MOSH_ENVIRONMENT_H

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <functional>
#include "mosh_builtins.h"

extern std::map<std::string, std::function<int(std::vector<std::string>)>> BUILTINS;
extern std::vector<std::string> PATH;

#endif
