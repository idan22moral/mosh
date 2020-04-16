#include "parser_utils.h"
#include <ctype.h>
std::vector<std::string> tokenize(std::string s)
{
	int i, slen;
	parse_mode mode = parse_mode::NORMAL;
	std::vector<std::string> tokens;
	std::stringstream current_token;

	slen = s.length();
	for (i = 0; i < slen; i++)
	{
		switch (mode)
		{
		case parse_mode::NORMAL:
			if (s[i] == '\'') // start of ' literal
				mode = parse_mode::APOSTROPHE;
			else if (s[i] == '\"') // start of " literal
				mode = parse_mode::QUOTATION;
			else if (s[i] == ' ' || s[i] == '\t' || ispunct(s[i])) // end of token
			{
				tokens.push_back(current_token.str());
				current_token.str(std::string()); // clear the stream
				tokens.push_back(std::string(1, s[i]));
			}
			else
				current_token << s[i];
			
			break;

		case parse_mode::APOSTROPHE:
			if (s[i] == '\'') // end of ' literal
				mode = parse_mode::NORMAL;
			else
				current_token << s[i];
			break;

		case parse_mode::QUOTATION:
			if (s[i] == '\"') // end of " literal
				mode = parse_mode::NORMAL;
			// ********** Add string embedded special chars support here **********
			else
				current_token << s[i];
			break;
		
		default:
			break;
		}
	}

	if (!current_token.str().empty())
		tokens.push_back(current_token.str());

	return tokens;
}
