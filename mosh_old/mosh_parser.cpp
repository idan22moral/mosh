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
	if (op == ";")
		return token_label::SEMICOLON;
	return token_label::UNDEFINED;
}

std::vector<std::string> tokenize(std::string input)
{
	int i, slen;
	parse_mode mode = parse_mode::NORMAL;
	std::vector<std::string> tokens;
	std::stringstream current_token;

	slen = input.length();
	for (i = 0; i < slen; i++)
	{
		switch (mode)
		{
		case parse_mode::NORMAL:
		{
			if (input[i] == '\'') // start of ' literal
			{
				mode = parse_mode::APOSTROPHE;
			}
			else if (input[i] == '\"') // start of " literal
			{
				mode = parse_mode::QUOTATION;
			}
			else if (input[i] == ' ' || input[i] == '\t') // end of token
			{
				save_token(tokens, current_token);
			}
			else if (is_operator(input[i]))
			{
				save_token(tokens, current_token);

				// the current and next chars are operators
				if (i + 1 < slen && is_operator(input[i + 1]))
				{
					// the current and next chars form an operator (e.g. '|' + '|' = "||")
					if (is_operator(input.substr(i, 2)))
					{
						tokens.push_back(input.substr(i, 2));
						i++;
						break; // force switch ending
					}
					else
					{
						// invalid operator (e.g. "|&")
						throw mosh_syntax_error("invalid operator - " + input.substr(i, 2));
					}
				}
				else
				{
					tokens.push_back(std::string(1, input[i]));
				}
			}
			else
			{
				current_token << input[i];
			}
		}
		break;

		case parse_mode::APOSTROPHE:
		{
			if (input[i] == '\'') // end of ' literal
			{
				mode = parse_mode::NORMAL;
			}
			else
			{
				current_token << input[i];
			}
		}
		break;

		case parse_mode::QUOTATION:
		{
			if (input[i] == '\"') // end of " literal
			{
				mode = parse_mode::NORMAL;
			}
			// ********** Add string embedded special chars support here **********
			else
			{
				current_token << input[i];
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
					mosh_internal_error("unknown operator \"" + tokens[i] + "\" while creating labeling token");
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
	mosh_command *cmd = nullptr;
	mosh_pipe *pipe = nullptr;
	bool save_pipe = false, save_cmd = false, set_right_cmd = false;
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
				throw mosh_internal_error("cannot add argument to a command that was not allocated");
			}
		}
		break;

		case token_label::COMMAND:
		{
			cmd = new mosh_command(current_token.first);
			if (set_right_cmd && pipe)
			{
				pipe->set_right_command(cmd);
				set_right_cmd = false;
			}
			else
			{
				ast_list.push_back(cmd);
			}
		}
		break;

		case token_label::PIPE:
		{
			// make sure there's something to pipe to
			if (ast_list.size() > 0)
			{
				if (!ast_list.back()->sealed())
				{
					pipe = new mosh_pipe();
					pipe->set_left_command(ast_list.back());

					ast_list.pop_back();
					ast_list.push_back(pipe);

					// pipe is valid only if both commands are set
					set_right_cmd = true;
				}
				else
				{
					throw mosh_syntax_error("command was not specified before a pipe");
				}
			}
			else
			{
				throw mosh_syntax_error("command was not specified before a pipe");
			}
		}
		break;

		case token_label::SEMICOLON: // end of tree
		{
			if (ast_list.size() > 0)
			{
				ast_list.back()->seal();
			}
			else
			{
				throw mosh_syntax_error("unexpected token `;`");
			}
		}
		break;

		case token_label::OR:
		case token_label::DETACH:
		case token_label::AND:
		{
			throw mosh_internal_error("operator \"" + current_token.first + "\" is supported yet");
		}
		break;

		case token_label::UNDEFINED:
		{
			throw mosh_internal_error("type of \"" + current_token.first + "\" is undefined");
		}
		break;

		default:
			throw mosh_internal_error("unknown operator \"" + current_token.first + "\" while creating AST");
			break;
		}
	}

	return ast_list;
}

std::vector<mosh_ast_node *> parse_input(std::string user_input)
{
	std::vector<std::string> tokens;
	std::vector<std::pair<std::string, token_label>> labeled_tokens;
	std::vector<mosh_ast_node *> ast_list;

	// Tokenize the input
	tokens = tokenize(user_input);
	debug_tokens(tokens);

	// Label the tokens
	labeled_tokens = label_tokens(tokens);
	debug_labeled_tokens(labeled_tokens);

	// Build list of ASTs from the labeled tokens
	ast_list = build_ast_list(labeled_tokens);
	debug_ast_list(ast_list);

	return ast_list;
}