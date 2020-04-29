#include "mosh_ast_node.h"

void mosh_ast_node::seal()
{
	_sealed = true;
}

bool mosh_ast_node::sealed()
{
	return _sealed;
}