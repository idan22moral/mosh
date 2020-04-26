#ifndef _MOSH_PIPE_NODE_H
#define _MOSH_PIPE_NODE_H

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <filesystem>
#include <unistd.h>
#include <exception>
#include <iostream>
#include "mosh_environment.h"
#include "mosh_ast_node.h"
#include "mosh_command.h"

class mosh_pipe : public mosh_ast_node
{
private:
	mosh_command _first;
	mosh_command _second;
public:
	// Constructors
	mosh_pipe();
	mosh_pipe(mosh_command first, mosh_command second);

	// Setters
	void set_first_command(mosh_command cmd);
	void set_second_command(mosh_command cmd);

	// Getters
	void debug();

	virtual int execute();
};

#endif
