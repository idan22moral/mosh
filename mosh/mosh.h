#ifndef MOSH_H
#define MOSH_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _STR(x) #x
#define STR(x) _STR(x)

#define ARG_MAX 2097152
#define ARG_MAX_STR STR(ARG_MAX)

void print_prompt();
char* get_input();
char** split_string(char* s, const char* sep);
char** split_into_commands(char* buffer);
void mosh_interactive();
#endif //MOSH_H
