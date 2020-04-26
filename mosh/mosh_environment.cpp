#include "mosh_environment.h"

std::map<std::string, std::function<int(std::vector<std::string>)>> BUILTINS{{"cd", cd}, {"set", set}};
std::vector<std::string> PATH{
	"/home/idan/.local/bin",
	"/usr/local/sbin",
	"/usr/local/bin",
	"/usr/sbin",
	"/usr/bin",
	"/sbin",
	"/bin",
	"/usr/games",
	"/usr/local/games",
	"/snap/bin"};
std::string PWD;