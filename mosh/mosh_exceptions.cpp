#include "mosh_exceptions.h"

mosh_exception::mosh_exception(std::string prefix, std::string message) : runtime_error("")
{
	std::stringstream stream;
	stream << prefix << message;
	((std::runtime_error &)*this) = std::runtime_error(stream.str());
}
