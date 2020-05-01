#include "mosh.h"

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

void print_prompt()
{
	fwrite("$ ", 1, 2, stdout);
}

std::string get_input()
{
	std::string line;
	std::getline(std::cin, line, '\n');
	return line;
}

int execute_commands(std::vector<mosh_ast_node *> commands)
{
	for (auto &&command : commands)
	{
		try
		{
			command->execute();
		}
		catch(const mosh_exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	return 0;
}

int mosh_interactive()
{
	int i = 0;
	std::string line;
	std::vector<std::string> tokens;
	std::vector<std::pair<std::string, token_label>> labeled_tokens;
	std::vector<mosh_ast_node *> ast;

	while (true)
	{
		print_prompt();
		line = get_input();

		if (std::cin.eof())
			return 1;
		
		try
		{
			// Convert the raw input into tokens
			tokens = tokenize(line);

			std::cout << "Tokenize:\n[ ";
			// Loop through the recieved commands
			for (i = 0; i < tokens.size(); i++)
			{
				std::cout << "\'" << tokens[i] << "\', ";
			}
			std::cout << " ]\n" << std::endl;
		}
		catch (const mosh_exception &e)
		{
			std::cerr << e.what() << '\n';
			continue;
		}

		try
		{
			labeled_tokens = label_tokens(tokens);

			std::cout << "Label:\n[ ";
			// Loop through the recieved commands
			for (i = 0; i < labeled_tokens.size(); i++)
			{
				std::cout << "(\'" << labeled_tokens[i].first << "\', " << token_label_str(labeled_tokens[i].second) << "), ";
			}
			std::cout << " ]\n" << std::endl;
		}
		catch(const mosh_exception &e)
		{ 
			std::cerr << e.what() << '\n';
		}
		

		try
		{
			ast = build_ast_list(labeled_tokens);

			for (i = 0; i < ast.size(); i++)
			{
				auto node = ast[i];
				mosh_command *cmd = dynamic_cast<mosh_command *>(node);
				if (cmd)
				{
					cmd->debug();
				}
				else
				{
					mosh_pipe *pipe = dynamic_cast<mosh_pipe *>(node);
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
		catch(const mosh_exception& e)
		{
			std::cerr << e.what() << '\n';
		}
		
		execute_commands(ast);

		// free the memory of the AST
		for (i = 0; i < ast.size(); i++)
		{
			delete ast[i];
			ast[i] = nullptr;
		}
		ast.clear();
		tokens.clear();
		
	}

	return 0;
}