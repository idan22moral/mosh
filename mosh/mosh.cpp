#include "mosh.h"

char input_buffer[ARG_MAX];

void print_prompt()
{
	fwrite("$ ", 1, 2, stdout);
}

char* get_input()
{
	fgets(input_buffer, ARG_MAX, stdin);
	return input_buffer;
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
		commands = split_into_commands(input_ptr);

		if (commands == NULL)
			return 1;

		while ((cmd_ptr = commands[i++]) != NULL)
		{
			puts(cmd_ptr);
		}
	}
	
	return 0;
}