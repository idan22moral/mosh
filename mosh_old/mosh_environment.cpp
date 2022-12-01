#include <unistd.h>
#include "mosh_environment.h"

#define MAX_USERNAME_LEN 256

std::string login_username() {
	char username[MAX_USERNAME_LEN];
	getlogin_r(username, MAX_USERNAME_LEN);
	return std::string(username);
}

std::map<std::string, std::function<int(std::vector<std::string>)>> BUILTINS{{"cd", cd}, {"boop", mosh_exit}};
std::vector<std::string> PATH{
	"/home/" + login_username() + "/.local/bin",
	"/usr/local/sbin",
	"/usr/local/bin",
	"/usr/sbin",
	"/usr/bin",
	"/sbin",
	"/bin",
	"/usr/games",
	"/usr/local/games",
	"/snap/bin"};
