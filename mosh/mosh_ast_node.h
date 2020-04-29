#ifndef _MOSH_AST_NODE_H
#define _MOSH_AST_NODE_H


class mosh_ast_node
{
private:
	bool _sealed;

public:
	virtual int execute() = 0;
	virtual void debug() = 0;
	void seal();
	bool sealed();
};

#endif
