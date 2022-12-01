#include "mosh_ast_node.h"

mosh_ast_node::mosh_ast_node()
{
	_sealed = false;
}

void mosh_ast_node::seal()
{
	_sealed = true;
}

bool mosh_ast_node::sealed()
{
	return _sealed;
}