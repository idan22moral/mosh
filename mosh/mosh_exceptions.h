#ifndef _MOSH_EXCEPTIONS_H
#define _MOSH_EXCEPTIONS_H


#include <exception>
#include <string>
#include <sstream>


class mosh_exception : public std::runtime_error
{
public:
	mosh_exception(std::string prefix, std::string message);
};


class mosh_syntax_error : public mosh_exception
{
public:
	mosh_syntax_error() : mosh_syntax_error("unknown")
	{
	}

	mosh_syntax_error(std::string message) : mosh_exception("Syntax Error: ", message)
	{
	}
};


class mosh_internal_error : public mosh_exception
{
public:
	mosh_internal_error() : mosh_internal_error("unknown")
	{
	}

	mosh_internal_error(std::string message) : mosh_exception("Internal Error: ", message)
	{
	}
};

#endif