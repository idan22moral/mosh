#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>

enum class parse_mode {
	NORMAL,
	APOSTROPHE,	// '
	QUOTATION	// "
};

enum class token_type {
	EXECUTABLE,
	ARGUMENT,
	OPERATOR
}

bool is_operator(char ch);
std::vector<std::string> tokenize(std::string s);
std::vector<std::pair<std::string, token_type>> label_tokens(std::vector<std::string> tokens);

#endif //PARSER_UTILS_H
