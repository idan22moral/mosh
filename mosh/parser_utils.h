#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

enum class parse_mode {
	NORMAL,
	APOSTROPHE,	// '
	QUOTATION	// "
};

bool is_operator(char ch);
std::vector<std::string> tokenize(std::string s);

#endif //PARSER_UTILS_H
