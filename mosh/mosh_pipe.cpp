#include "mosh_pipe.h"

mosh_pipe::mosh_pipe() : _first(new mosh_command("")), _second(new mosh_command(""))
{
}

mosh_pipe::mosh_pipe(mosh_ast_node *first, mosh_ast_node *second) : _first(first), _second(second)
{
}

mosh_pipe::~mosh_pipe()
{
	delete _first;
	_first = nullptr;
	delete _second;
	_second = nullptr;
}

void mosh_pipe::set_left_command(mosh_ast_node *cmd)
{
	if (_first != nullptr)
		delete _first;
	_first = cmd;
}

void mosh_pipe::set_right_command(mosh_ast_node *cmd)
{
	if (_second != nullptr)
		delete _second;
	_second = cmd;
}

void mosh_pipe::debug()
{
	std::cout << "--------------------------------------------------" << std::endl;
	std::cout << "Pipe:" << std::endl;
	std::cout << "First:" << std::endl;
	_first->debug();
	std::cout << "Second:" << std::endl;
	_second->debug();
	std::cout << "--------------------------------------------------" << std::endl;
}

int mosh_pipe::execute()
{
	int result, pipe_fds[2], child_pid;

	result = pipe(pipe_fds);

	if (result)
	{
		throw mosh_internal_error("pipe creation failed on pipe execution (first)");
	}

	// left child (reader)
	switch (fork())
	{
	case -1:
		throw mosh_internal_error("fork failed on pipe execution (second fork)");
	case 0:											// child
		close(pipe_fds[STDOUT_FILENO]);				// close useless allocated stdout
		dup2(pipe_fds[STDIN_FILENO], STDIN_FILENO); // move pipe read to stdin
		close(pipe_fds[STDIN_FILENO]);				// close old stdin

		try
		{
			result = _second->execute();
		}
		catch (const mosh_exception &e)
		{
			std::cerr << e.what() << '\n';
			result = EXIT_FAILURE;
		}
		exit(result);

	default: // parent
		break;
	}

	// right child (writer)
	child_pid = fork();
	switch (child_pid)
	{
	case -1:
		throw mosh_internal_error("fork failed on pipe execution (first fork)");
	case 0:											  // child
		close(pipe_fds[STDIN_FILENO]);				  // close useless allocated stdin
		dup2(pipe_fds[STDOUT_FILENO], STDOUT_FILENO); // move write to stdout
		close(pipe_fds[STDOUT_FILENO]);				  // close old stdout

		try
		{
			result = _first->execute();
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
			result = EXIT_FAILURE;
		}
		exit(result);

	default: // parent
		break;
	}

	// the parent does not need the pipe
	close(pipe_fds[STDIN_FILENO]);
	close(pipe_fds[STDOUT_FILENO]);

	// wait for both children to finish
	while (wait(NULL) > 0);

	return EXIT_SUCCESS;
}
