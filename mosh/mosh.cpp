#include "mosh.h"

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

int mosh_interactive()
{
	int i = 0;
	std::string line;
	std::vector<std::string> tokens;

	while (true)
	{
		print_prompt();
		line = get_input();

		if (std::cin.eof())
			return 1;

		// Convert the raw input into tokens
		tokens = tokenize(line);

		// Loop through the recieved commands
		for (i = 0; i < tokens.size(); i++)
		{
			std::cout << "\'" << tokens[i] << "\'" << std::endl;
		}
	}

	return 0;
}