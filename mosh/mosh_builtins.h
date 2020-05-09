#ifndef _MOSH_BUILTINS_H
#define _MOSH_BUILTINS_H

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "mosh_builtins.h"

std::string _cwd();

int cd(std::vector<std::string> argv);
int mosh_exit(std::vector<std::string> argv);

#endif
