#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <string>
#include <sstream>
#include <ctype.h>

bool is_operator(char c);
bool is_operator(std::string str);
void save_token(std::vector<std::string> &tokens, std::stringstream &token_to_save);

#endif //PARSER_UTILS_H
