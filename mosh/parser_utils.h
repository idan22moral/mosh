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

enum class parse_mode {
	NORMAL,
	APOSTROPHE,	// '
	QUOTATION	// "
};

enum class token_label {
	COMMAND,
	ARGUMENT,
	OPERATOR,
	FILE,
	UNDEFINED
};

enum class mosh_operator {
	PIPE,					// |
	DETACH,					// &
	AND,					// &&
	OR,						// ||
	ROUND_BRACKET_OPEN,		// (
	ROUND_BRACKET_CLOSE,	// )
	REDIRECT_LEFT,			// >
	SEMICOLON,				// ;
	INVALID
};

bool is_operator(char c);
bool is_operator(std::string str);
void save_token(std::vector<std::string> &tokens, std::stringstream &token_to_save);
mosh_operator label_operator(std::string op);
token_label label_token_by_operator(mosh_operator op);
std::vector<std::string> tokenize(std::string s);
std::vector<std::pair<std::string, token_label>> label_tokens(std::vector<std::string> tokens);

#endif //PARSER_UTILS_H
