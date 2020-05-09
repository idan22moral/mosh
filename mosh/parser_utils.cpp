#include "parser_utils.h"

const std::vector<std::string> OPERATORS{"|", "&", "&&", "||", ";"};

bool is_operator(char c)
{
	return (bool)strchr("|&;", c);
}

bool is_operator(std::string str)
{
	// mosh operators has maximum length of 2 characters
	if (str.length() > 2)
		return false;
	return std::find(OPERATORS.begin(), OPERATORS.end(), str) != OPERATORS.end();
}

void save_token(std::vector<std::string> &tokens, std::stringstream &token_to_save)
{
	if (token_to_save.str() != "")
	{
		tokens.push_back(token_to_save.str());
	}

	// clear the stream
	token_to_save.str(std::string());
}
