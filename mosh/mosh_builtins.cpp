#include "mosh_builtins.h"

std::string _cwd()
{
	char buffer[FILENAME_MAX];
	getcwd(buffer, FILENAME_MAX);
	return std::string(buffer);
}

int cd(std::vector<std::string> argv)
{
	static std::string previous_dir = _cwd();
	std::string temp_dir_str;
	struct passwd *pw;
	int size = argv.size();
	int result = 0;

	if (size <= 0)
	{
		std::cerr << "invalid usage of 'cd'." << std::endl;
		return EXIT_FAILURE;
	}
	else if (size == 1) // cd
	{
		// change to user's home
		pw = getpwuid(getuid());
		result = chdir(pw->pw_dir);

		if (result)
		{
			perror("cd");
			return result;
		}
	}
	else // cd [...]
	{
		temp_dir_str = _cwd();

		// roll back to last directory
		if (argv[1] == "-")
		{
			result = chdir(previous_dir.data());

			if (result)
			{
				perror("cd");
				return result;
			}
			else
			{
				std::cout << previous_dir << std::endl;
			}
		}
		else // change directory to the given directory
		{
			result = chdir(argv[1].c_str());

			if (result)
			{
				perror("cd");
				return result;
			}
		}

		previous_dir = temp_dir_str;
	}

	return EXIT_SUCCESS;
}

int set(std::vector<std::string> argv)
{
	return 0;
}

int mosh_exit(std::vector<std::string> argv)
{
	exit(0);
}