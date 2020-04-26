#ifndef MOSH_H
#define MOSH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mosh_parser.h"
#include "mosh_ast_node.h"

#define _STR(x) #x
#define STR(x) _STR(x)

#define ARG_MAX 2097152
#define ARG_MAX_STR STR(ARG_MAX)

void print_prompt();
std::string get_input();
int mosh_executer(std::vector<mosh_ast_node> commands);
int mosh_interactive();

#endif //MOSH_H
