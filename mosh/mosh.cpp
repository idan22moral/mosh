#include "mosh.h"

bool debug_mode;

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
		catch (const mosh_exception &e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	return 0;
}

void parse_commandline_arguments(int argc, char *argv[])
{
	int i;

	for (i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-d")) // debug mode
		{
			debug_mode = true;
		}
	}
}

int mosh_interactive()
{
	int i = 0;
	std::string line;
	std::vector<mosh_ast_node *> ast_list;

	while (true)
	{
		print_prompt();
		line = get_input();

		if (std::cin.eof())
			return 1;
		
		// parse the input of the user
		try
		{
			ast_list = parse_input(line);
		}
		catch (const mosh_exception &e)
		{
			std::cerr << e.what() << '\n';
			continue;
		}

		// execute the whole AST commands
		execute_commands(ast_list);

		// free the memory of the AST
		for (i = 0; i < ast_list.size(); i++)
		{
			delete ast_list[i];
			ast_list[i] = nullptr;
		}
		ast_list.clear();
	}

	return 0;
}