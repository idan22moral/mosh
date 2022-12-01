#include <iostream>
#include "mosh.h"

int main(int argc, char *argv[])
{
	int result = 0;

	if (argc > 1)
	{
		parse_commandline_arguments(argc, argv);
	}

	result = mosh_interactive();
	puts("");

	return result;
}
