#include "mosh.h"

char input_buffer[ARG_MAX];

void print_prompt()
{
	fwrite("$ ", 1, 2, stdout);
}

char *get_input()
{
	return fgets(input_buffer, ARG_MAX, stdin);
}

int mosh_interactive()
{
	int i = 0;
	char *cmd_ptr = NULL;
	char *input_ptr = NULL;
	char **commands = NULL;

	while (true)
	{
		print_prompt();
		input_ptr = get_input();

		if (feof(stdin))
			return 1;

		commands = split_into_commands(input_ptr);

		if (commands == NULL)
			return 1;

		while ((cmd_ptr = commands[i++]) != NULL)
		{
			puts(cmd_ptr);
		}
		i = 0;
		
		free_tokens(commands);
		commands = NULL;
	}

	return 0;
}