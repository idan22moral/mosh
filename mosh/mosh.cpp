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

char **split_string(char *s, const char *sep)
{
	int token_count = 1;
	char *ptr = NULL;
	char **tokens = (char **)malloc(sizeof(char**));

	tokens[0] = strtok(s, sep);

	while ((ptr = strtok(NULL, sep)))
	{
		tokens[token_count] = ptr;
		tokens = (char **)realloc(tokens, ++token_count * sizeof(char**));
	}
	tokens[token_count] = NULL;

	return tokens;
}

char **split_into_commands(char* buffer)
{
	return split_string(buffer, ";");
}

void mosh_interactive()
{
	int i = 0;
	char *cmd_ptr = NULL;
	char *input_ptr = NULL;
	char **commands = NULL;

	print_prompt();
	input_ptr = get_input();
	commands = split_into_commands(input_ptr);

	if (commands == NULL)
		return 1;

	while ((cmd_ptr = commands[i++]) != NULL)
	{
		std::cout << cmd_ptr << std::endl;
	}
}