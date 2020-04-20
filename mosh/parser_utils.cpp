#include "parser_utils.h"

const std::vector<std::string> OPERATORS{"|", "&", "&&", "||", "(", ")", ">", ";"};

bool is_operator(char c)
{
	return (bool)strchr("|&()>;", c);
}

bool is_operator(std::string str)
{
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
	token_to_save.str(std::string()); // clear the stream
}

mosh_operator label_operator(std::string op)
{
	if (op == "|")
		return mosh_operator::PIPE;
	if (op == "&")
		return mosh_operator::DETACH;
	if (op == "&&")
		return mosh_operator::AND;
	if (op == "||")
		return mosh_operator::OR;
	if (op == ">")
		return mosh_operator::REDIRECT_LEFT;
	if (op == "(")
		return mosh_operator::ROUND_BRACKET_OPEN;
	if (op == ")")
		return mosh_operator::ROUND_BRACKET_CLOSE;
	if (op == ";")
		return mosh_operator::SEMICOLON;
	return mosh_operator::INVALID;
}

token_label label_token_by_operator(mosh_operator op)
{
	switch (op)
	{
		case mosh_operator::REDIRECT_LEFT:
			return token_label::FILE;
		case mosh_operator::ROUND_BRACKET_CLOSE:
			return token_label::OPERATOR;
		case mosh_operator::ROUND_BRACKET_OPEN:
			return token_label::UNDEFINED;
		default:
			return token_label::COMMAND;
	}
}

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
		{
			if (s[i] == '\'') // start of ' literal
			{
				mode = parse_mode::APOSTROPHE;
			}
			else if (s[i] == '\"') // start of " literal
			{
				mode = parse_mode::QUOTATION;
			}
			else if (s[i] == ' ' || s[i] == '\t') // end of token
			{
				save_token(tokens, current_token);
			}
			else if (is_operator(s[i]))
			{
				save_token(tokens, current_token);
				tokens.push_back(std::string(1, s[i]));
			}
			else
			{
				current_token << s[i];
			}
		}
		break;

		case parse_mode::APOSTROPHE:
		{
			if (s[i] == '\'') // end of ' literal
			{
				mode = parse_mode::NORMAL;
			}
			else
			{
				current_token << s[i];
			}
		}
		break;

		case parse_mode::QUOTATION:
		{
			if (s[i] == '\"') // end of " literal
			{
				mode = parse_mode::NORMAL;
			}
			// ********** Add string embedded special chars support here **********
			else
			{
				current_token << s[i];
			}
		}
		break;

		default:
			break;
		}
	}

	// Save the token if it's not empty
	if (!current_token.str().empty())
	{
		tokens.push_back(current_token.str());
	}

	return tokens;
}

std::vector<std::pair<std::string, token_label>> label_tokens(std::vector<std::string> tokens)
{
	std::vector<std::pair<std::string, token_label>> labeled_tokens;
	std::pair<std::string, token_label> current;
	int i, token_count = tokens.size();
	token_label next_type = token_label::COMMAND;

	if (token_count > 0)
	{
		for (int i = 0; i < token_count; i++)
		{
			current = std::pair<std::string, token_label>();
			current.first = tokens[i];

			switch (next_type)
			{
			case token_label::COMMAND:
			{
				next_type = token_label::ARGUMENT;
				current.second = token_label::COMMAND;
			}
			break;

			case token_label::ARGUMENT:
			{
				next_type = token_label::UNDEFINED;
				current.second = token_label::ARGUMENT;
			}
			break;

			case token_label::FILE:
			{
				next_type = token_label::ARGUMENT;
				current.second = token_label::FILE;
			}
			break;

			case token_label::UNDEFINED:
			default:
			{
				if (is_operator(tokens[i]))
				{
					next_type = label_token_by_operator(label_operator(tokens[i]));
					current.second = token_label::OPERATOR;
				}
				else
				{
					current.second = token_label::UNDEFINED;
				}
			}
			break;
			}

			labeled_tokens.push_back(current);
		}
	}

	return labeled_tokens;
}
