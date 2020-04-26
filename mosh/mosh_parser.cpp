#include "mosh_parser.h"

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
	/*if (op == ">")
		return mosh_operator::REDIRECT_LEFT;
	if (op == "(")
		return mosh_operator::ROUND_BRACKET_OPEN;
	if (op == ")")
		return mosh_operator::ROUND_BRACKET_CLOSE;*/
	if (op == ";")
		return mosh_operator::SEMICOLON;
	return mosh_operator::INVALID;
}

token_label label_token_by_operator(mosh_operator op)
{
	switch (op)
	{
	/*case mosh_operator::REDIRECT_LEFT:
		return token_label::FILE;
	case mosh_operator::ROUND_BRACKET_CLOSE:
		return token_label::OPERATOR;
	case mosh_operator::ROUND_BRACKET_OPEN:
		return token_label::UNDEFINED;*/
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

std::vector<mosh_ast_node*> build_ast_list(std::vector<std::pair<std::string, token_label>> labeled_tokens)
{
	std::vector<mosh_ast_node*> ast_list;
	std::vector<std::string> args;
	std::pair<std::string, token_label> current_token;
	mosh_command* cmd;
	mosh_pipe* pipe;
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
				cmd->add_argument(labeled_tokens[i].first);
			}
			else
			{
				puts("Command not allocated.");
			}
		}
		break;

		case token_label::COMMAND:
		{
			//cmd.set_command(labeled_tokens[i].first);
			cmd = new mosh_command(labeled_tokens[i].first);
			save_cmd = true;
		}
			
		break;

		/*case token_label::FILE:
		{
			if (i == 0 || last_redirect_index != i - 1)
			{
				perror("token parsed as file");
			}
			if (label_operator(labeled_tokens[i - 1].first) == mosh_operator::REDIRECT_LEFT)
			{
				
			}
		}
		break;*/

		case token_label::OPERATOR:
		{
			switch (label_operator(current_token.first))
			{
			case mosh_operator::PIPE:
				if (save_cmd)
				{
					if(pipe)
					{
						pipe->set_first_command(*cmd);
					}
					else
					{
						puts("Pipe not allocated.");
					}
					
				}
				break;
			case mosh_operator::SEMICOLON: // end of tree
				if (save_pipe)
				{
					if (pipe)
					{
						pipe->set_second_command(*cmd);
					}
					else
					{
						puts("Pipe not allocated.");
					}
					ast_list.push_back(pipe);
					save_pipe = false;
					pipe = new mosh_pipe();
				}
				else if(save_cmd)
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
			case mosh_operator::OR:
				break;
			case mosh_operator::DETACH:
				break;
			case mosh_operator::AND:
				break;
			case mosh_operator::INVALID:
				perror("Invalid operator");
				break;
			default:
				break;
			}
		}
		break;

		case token_label::UNDEFINED:
		{
			perror("Undefined token type");
		}
		break;
		
		default:
		{
			perror("Undefined token type (default)");
		}
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
			puts("Pipe not allocated.");
		}
		ast_list.push_back(pipe);
	} 
	
	return ast_list;
}