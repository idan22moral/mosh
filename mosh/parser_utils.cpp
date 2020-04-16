#include "parser_utils.h"

char **split_string(char *s, const char *sep)
{
	int token_count = 2;
	char *ptr = NULL;
	char **tokens = (char **)malloc(2 * sizeof(char *));

	tokens[0] = strtok(s, sep);

	while ((ptr = strtok(NULL, sep)))
	{
		tokens[token_count - 1] = ptr;
		tokens = (char **)realloc(tokens, ++token_count * sizeof(char *));
	}
	tokens[token_count - 1] = NULL;

	return tokens;
}

char **split_into_commands(char *buffer)
{
	return split_string(buffer, ";");
}

void free_tokens(char **tokens)
{
	int i = 0;
	if (tokens)
		free(tokens);
}