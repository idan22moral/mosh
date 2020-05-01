#include "mosh_debug.h"

std::string token_label_str(token_label label)
{
	switch (label)
	{
	case token_label::AND:
		return "AND";
	case token_label::ARGUMENT:
		return "ARGUMENT";
	case token_label::COMMAND:
		return "COMMAND";
	case token_label::DETACH:
		return "DETACH";
	case token_label::OR:
		return "OR";
	case token_label::PIPE:
		return "PIPE";
	case token_label::SEMICOLON:
		return "SEMICOLON";
	case token_label::UNDEFINED:
		return "UNDEFINED";
	default:
		return "ERROR!";
	}
}

void debug_tokens(std::vector<std::string> tokens)
{
	int i, size;

	if (!debug_mode)
		return;

	size = tokens.size();

	std::cout << "Tokenize:\n[ ";

	for (i = 0; i < size - 1; i++)
	{
		std::cout << "\'" << tokens[i] << "\', ";
	}

	if (i) // the vector was not empty
	{
		std::cout << '\'' << tokens[i] << '\'';
	}

	std::cout << " ]\n"
			  << std::endl;
}

void debug_labeled_tokens(std::vector<std::pair<std::string, token_label>> labeled_tokens)
{
	int i, size;

	if (!debug_mode)
		return;

	size = labeled_tokens.size();

	std::cout << "Label:\n[ ";

	for (i = 0; i < size - 1; i++)
	{
		std::cout << "(\'" << labeled_tokens[i].first << "\', " << token_label_str(labeled_tokens[i].second) << "), ";
	}

	if (i) // the vector was not empty
	{
		std::cout << "(\'" << labeled_tokens[i].first << "\', " << token_label_str(labeled_tokens[i].second) << ')';
	}

	std::cout << " ]\n"
			  << std::endl;
}

void debug_ast_list(std::vector<mosh_ast_node *> ast_list)
{
	mosh_command *cmd = nullptr;
	mosh_pipe *pipe = nullptr;
	mosh_ast_node *ast_list_node = nullptr;
	int i, size;

	if (!debug_mode)
		return;

	size = ast_list.size();
	for (i = 0; i < size; i++)
	{
		ast_list_node = ast_list[i];
		cmd = dynamic_cast<mosh_command *>(ast_list_node);

		if (cmd)
		{
			cmd->debug();
		}
		else
		{
			pipe = dynamic_cast<mosh_pipe *>(ast_list_node);
			if (pipe)
			{
				pipe->debug();
			}
			else
			{
				throw mosh_internal_error("ast node was not command/pipe.");
			}
		}
	}
}