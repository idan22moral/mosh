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
	mosh_ast_node* _first;
	mosh_ast_node* _second;
public:
	// Constructors
	mosh_pipe();
	mosh_pipe(mosh_ast_node* first, mosh_ast_node* second);
	~mosh_pipe();

	// Setters
	void set_left_command(mosh_ast_node* cmd);
	void set_right_command(mosh_ast_node* cmd);

	// Getters
	void debug();

	virtual int execute();
};

#endif
