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

enum class parse_mode {
	NORMAL,
	APOSTROPHE,	// '
	QUOTATION	// "
};

enum class token_label {
	COMMAND,
	ARGUMENT,
	//OPERATOR,
	//FILE,
	PIPE,					// |
	DETACH,					// &
	AND,					// &&
	OR,						// ||
	SEMICOLON,				// ;
	UNDEFINED
	//ROUND_BRACKET_OPEN,	// (
	//ROUND_BRACKET_CLOSE,	// )
	//REDIRECT_LEFT,		// >
};

/*enum class mosh_operator {
	PIPE,					// |
	DETACH,					// &
	AND,					// &&
	OR,						// ||
	//ROUND_BRACKET_OPEN,		// (
	//ROUND_BRACKET_CLOSE,	// )
	//REDIRECT_LEFT,			// >
	SEMICOLON,				// ;
	INVALID
};*/

token_label label_operator(std::string op);
std::vector<std::string> tokenize(std::string s);
std::vector<std::pair<std::string, token_label>> label_tokens(std::vector<std::string> tokens);
std::vector<mosh_ast_node*> build_ast_list(std::vector<std::pair<std::string, token_label>> labeled_tokens);