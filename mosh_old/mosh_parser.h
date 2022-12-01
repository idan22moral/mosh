#ifndef _MOSH_PARSER_H
#define _MOSH_PARSER_H

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <string>
#include <sstream>
#include <ctype.h>
#include "mosh_exceptions.h"
#include "parser_utils.h"
#include "mosh_ast_node.h"
#include "mosh_pipe.h"
#include "mosh_command.h"
#include "mosh_debug.h"

enum class parse_mode
{
	NORMAL,
	APOSTROPHE, // '
	QUOTATION	// "
};

enum class token_label
{
	COMMAND,
	ARGUMENT,
	PIPE,	   // |
	DETACH,	   // &
	AND,	   // &&
	OR,		   // ||
	SEMICOLON, // ;
	UNDEFINED
};

token_label label_operator(std::string op);
std::vector<std::string> tokenize(std::string input);
std::vector<std::pair<std::string, token_label>> label_tokens(std::vector<std::string> tokens);
std::vector<mosh_ast_node *> build_ast_list(std::vector<std::pair<std::string, token_label>> labeled_tokens);
std::vector<mosh_ast_node *> parse_input(std::string user_input);

#endif