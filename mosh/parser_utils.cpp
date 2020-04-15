#include "parser_utils.h"

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