#include <exception>
#include <string>

class mosh_exception : std::exception
{
private:
	std::string _prefix;
	std::string _message;

public:
	mosh_exception(std::string prefix, std::string message)
	{
		_prefix = prefix;
		_message = message;
	}

	const char *what() const throw()
	{
		return (_prefix + _message).c_str();
	}
};

class mosh_syntax_error : mosh_exception
{
public:
	mosh_syntax_error() : mosh_syntax_error("unknown")
	{
	}

	mosh_syntax_error(std::string message) : mosh_exception("Syntax Error: ", message)
	{
	}
};

class mosh_internal_error : mosh_exception
{
public:
	mosh_internal_error() : mosh_internal_error("unknown")
	{
	}

	mosh_internal_error(std::string message) : mosh_exception("Internal Error: ", message)
	{
	}
};