#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <stdlib.h>
#include <string.h>

char **split_string(char *s, const char *sep);
char **split_into_commands(char *buffer);
void free_tokens(char **tokens);

#endif //PARSER_UTILS_H