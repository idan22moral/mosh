#include "mosh_pipe.h"

mosh_pipe::mosh_pipe() : _first(mosh_command("")), _second(mosh_command(""))
{
}

mosh_pipe::mosh_pipe(mosh_command first, mosh_command second) : _first(first), _second(second)
{
}

void mosh_pipe::set_first_command(mosh_command cmd)
{
	_first = cmd;
}

void mosh_pipe::set_second_command(mosh_command cmd)
{
	_second = cmd;
}

void mosh_pipe::debug()
{
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "Pipe:" << std::endl;
	std::cout << "First:" << std::endl;
	_first.debug();
	std::cout << "Second:" << std::endl;
	_second.debug();
	std::cout <<"--------------------------------------------------" << std::endl;
}

int mosh_pipe::execute()
{
	int result, fds_first[2], fds_second[2];

	result = pipe(fds_first);

	if (result == 0)
	{
		perror("pipe failed");
		return EXIT_FAILURE;
	}

	result = pipe(fds_second);

	if (result == 0)
	{
		perror("pipe failed");
		return EXIT_FAILURE;
	}

	// first child (writer)
	switch (fork())
	{
	case -1:
		perror("fork failed");
		return EXIT_FAILURE;
	case 0:	// child
		close(fds_first[STDIN_FILENO]);				   // close useless newly opened stdin
		dup2(fds_first[STDOUT_FILENO], STDOUT_FILENO); // move write to stdout
		close(fds_first[STDOUT_FILENO]);			   // close old stdout

		result = _first.execute();
		exit(result);
		break;
	default: // parent
		break;
	}

	// second child (reader)
	switch (fork())
	{
	case -1:
		perror("fork failed");
		return EXIT_FAILURE;
	case 0:	// child
		close(fds_second[STDOUT_FILENO]);			 // close useless newly opened stdout
		dup2(fds_first[STDIN_FILENO], STDIN_FILENO); // move read to stdin
		close(fds_first[STDIN_FILENO]);				 // close old stdin

		result = _second.execute();
		exit(result);
		break;
	default: // parent
		break;
	}

	return EXIT_SUCCESS;
}
