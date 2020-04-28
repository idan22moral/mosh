#include "mosh_parser.h"

token_label label_operator(std::string op)
{
	if (op == "|")
		return token_label::PIPE;
	if (op == "&")
		return token_label::DETACH;
	if (op == "&&")
		return token_label::AND;
	if (op == "||")
		return token_label::OR;
	/*if (op == ">")
		return mosh_operator::REDIRECT_LEFT;
	if (op == "(")
		return mosh_operator::ROUND_BRACKET_OPEN;
	if (op == ")")
		return mosh_operator::ROUND_BRACKET_CLOSE;*/
	if (op == ";")
		return token_label::SEMICOLON;
	return token_label::UNDEFINED;
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

				// the current and next chars are operators
				if (i + 1 < slen && is_operator(s[i + 1]))
				{
					// the current and next chars form an operator (e.g. '|' + '|' = "||")
					if (is_operator(s.substr(i, 2)))
					{
						tokens.push_back(s.substr(i, 2));
						i++;
						break; // force switch ending
					}
					else
					{
						// invalid operator (e.g. "|&")
						throw mosh_syntax_error("invalid operator - " + s.substr(i, 2));
					}
				}
				else
				{
					tokens.push_back(std::string(1, s[i]));
				}
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
	token_label operator_type;
	token_label prev_type = token_label::UNDEFINED;

	if (token_count > 0)
	{
		for (int i = 0; i < token_count; i++)
		{
			current = std::pair<std::string, token_label>();
			current.first = tokens[i];

			if (is_operator(tokens[i]))
			{
				operator_type = label_operator(tokens[i]);
				switch (operator_type)
				{
				case token_label::PIPE:
				case token_label::DETACH:
				case token_label::AND:
				case token_label::OR:
				case token_label::SEMICOLON:
				{
					if (prev_type == token_label::COMMAND || prev_type == token_label::ARGUMENT) // valid
					{
						current.second = operator_type;
						prev_type = operator_type;
					}
					else
					{
						throw mosh_syntax_error("invalid operator location");
					}
				}
				break;

				default:
					mosh_internal_error("unknown operator \"" + tokens[i] + "\" while creating labeling token.");
					break;
				}
			}
			else // token is not a mosh-operator
			{
				switch (prev_type)
				{
				case token_label::COMMAND:
				case token_label::ARGUMENT:
				{
					// must be an argument
					current.second = token_label::ARGUMENT;
					prev_type = token_label::ARGUMENT;
				}
				break;

				case token_label::PIPE:
				case token_label::DETACH:
				case token_label::AND:
				case token_label::OR:
				case token_label::SEMICOLON:
				case token_label::UNDEFINED:
				{
					// must be a command
					current.second = token_label::COMMAND;
					prev_type = token_label::COMMAND;
				}
				break;

				default:
					break;
				}
			}

			labeled_tokens.push_back(current);
		}
	}

	return labeled_tokens;
}

std::vector<mosh_ast_node *> build_ast_list(std::vector<std::pair<std::string, token_label>> labeled_tokens)
{
	std::vector<mosh_ast_node *> ast_list;
	std::vector<std::string> args;
	std::pair<std::string, token_label> current_token;
	mosh_command *cmd;
	mosh_pipe *pipe;
	bool save_pipe = false, save_cmd = false;
	int i, token_count = labeled_tokens.size(), last_redirect_index = -1;

	for (i = 0; i < token_count; i++)
	{
		current_token = labeled_tokens[i];

		switch (current_token.second)
		{
		case token_label::ARGUMENT:
		{
			if (cmd)
			{
				cmd->add_argument(current_token.first);
			}
			else
			{
				mosh_internal_error("cannot add argument to a command that was not allocated.");
			}
		}
		break;

		case token_label::COMMAND:
		{
			cmd = new mosh_command(current_token.first);
			save_cmd = true;
		}
		break;

			/*case token_label::FILE:
		{
			if (i == 0 || last_redirect_index != i - 1)
			{
				puts("token parsed as file");
			}
			if (label_operator(labeled_tokens[i - 1].first) == mosh_operator::REDIRECT_LEFT)
			{
				
			}
		}
		break;*/

		case token_label::PIPE:
			if (save_cmd)
			{
				if (pipe)
				{
					pipe->set_first_command(*cmd);
				}
				else
				{
					mosh_internal_error("cannot set 1st command of pipe that was not allocated.");
				}
			}
			break;
		case token_label::SEMICOLON: // end of tree
			if (save_pipe)
			{
				if (pipe)
				{
					pipe->set_second_command(*cmd);
				}
				else
				{
					mosh_internal_error("cannot set 2nd command of pipe that was not allocated.");
				}
				ast_list.push_back(pipe);
				save_pipe = false;
				pipe = new mosh_pipe();
			}
			else if (save_cmd)
			{
				ast_list.push_back(cmd);
				save_cmd = false;
			}
			else
			{
				puts("Somthing's wrong");
			}
			cmd = new mosh_command("");
			break;

		/*case token_label::OR:
			break;
		case token_label::DETACH:
			break;
		case token_label::AND:
			break;*/

		case token_label::UNDEFINED:
		{
			mosh_internal_error("type of \"" + current_token.first + "\" is undefined.");
		}
		break;

		default:
			mosh_internal_error("unknown operator \"" + current_token.first + "\" while creating AST.");
			break;
		}
	}

	if (save_cmd && !save_pipe)
	{
		ast_list.push_back(cmd);
	}
	else if (save_pipe && save_cmd)
	{
		if (pipe)
		{
			pipe->set_second_command(*cmd);
		}
		else
		{
			mosh_internal_error("cannot set 2nd command of pipe that was not allocated.");
		}
		ast_list.push_back(pipe);
	}

	return ast_list;
}