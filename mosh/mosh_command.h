#ifndef _MOSH_COMMAND_H
#define _MOSH_COMMAND_H

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <filesystem>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include "mosh_environment.h"
#include "mosh_exceptions.h"
#include "mosh_ast_node.h"

class mosh_command : public mosh_ast_node
{
private:
	std::string _command;
	std::string _original_command;
	std::vector<std::string> _args;
	bool _builtin;

public:
	// Constructors
	mosh_command(std::string command);
	mosh_command(std::string command, std::vector<std::string> args);

	// Setters
	void set_command(std::string command);
	void add_argument(std::string arg);

	// Getters
	bool builtin();
	void debug();

	void resolve();
	virtual int execute();
};

#endif
