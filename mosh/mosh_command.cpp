#include "mosh_command.h"

mosh_command::mosh_command(std::string command) : mosh_command(command, std::vector<std::string>())
{
}

mosh_command::mosh_command(std::string command, std::vector<std::string> args)
	: _args(), _original_command(command), _command(command)
{
	// add the original command to the beginning of the args
	std::cout << "pushing original command: " << _original_command << std::endl;
	_args.push_back(_original_command);

	for (int i = 0; i < args.size(); i++)
	{
		_args.push_back(args[i]);
	}

	// resolve the literal command, and set _builtin if the command is a mosh-builtin
	resolve();
}

void mosh_command::set_command(std::string command)
{
	_command = command;
	_original_command = command;
	_args.clear();
	if (_args.size() > 0)
		_args[0] = _original_command;
	else
		_args.push_back(_original_command);

	resolve();
}

void mosh_command::add_argument(std::string arg)
{
	std::cout << "Pushing argument: " << arg << std::endl;
	_args.push_back(arg);
	std::cout << "Pushed: " << _args.back() << std::endl;
}

void mosh_command::resolve()
{
	std::string tmp_command_path;

	if (BUILTINS.find(_command) != BUILTINS.end()) // check if command is mosh-builtin
	{
		std::cout << "BUILTIN!" << std::endl;
		_builtin = true;
	}
	else
	{
		_builtin = false;
		
		if (std::filesystem::exists(PWD + "/" + _command)) // check if command in pwd
		{
			_command = PWD + "/" + _command;
		}
		else // check if command in one of the environment paths
		{
			for (int i = 0; i < PATH.size(); i++)
			{
				tmp_command_path = PATH[i] + "/" + _command;
				if (std::filesystem::exists(tmp_command_path))
				{
					_command = tmp_command_path;
					break;
				}
			}
		}
	}
	
}

bool mosh_command::builtin()
{
	return _builtin;
}

void mosh_command::debug()
{
	std::cout << "Resolved command: " << _command << std::endl;
	std::cout << "Original command: " << _original_command << std::endl;
	std::cout << "Builtin: "<< (_builtin ? "True" : "False") << std::endl;
	std::cout << "Args (" <<  _args.size() << "): [";
	for (int i = 0; i < _args.size(); i++)
	{
		std::cout << "\"" << _args[i] << "\"";
		if (i < _args.size() - 1)
			std::cout << ", ";
	}
	std::cout << "]" << std::endl;
}

int mosh_command::execute()
{
	std::vector<char *> c_args;
	int fds_first[2], fds_second[2], result;

	if (_builtin)
	{
		return BUILTINS[_command](_args);
	}
	else
	{
		// convert to proper args format
		for (auto &arg : _args)
		{
			c_args.push_back((char *)arg.c_str()); // unsafe?
		}
		return execv(_command.c_str(), c_args.data());
	}
}