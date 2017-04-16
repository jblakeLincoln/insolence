#include "command_parser.h"

bool ParseArgument::Long(long *ret, char **args)
{
	if(args == nullptr || *args == nullptr)
		return false;

	char *id_end = strchr(*args, ' ');

	if(id_end == nullptr)
		id_end = *args + strlen(*args);

	for(int i = 0; i < id_end - *args - 1; ++i)
		if(isdigit(*args[i]) == 0)
			return false;

	*ret = strtol(*args, NULL, 10);
	*args = id_end;

	if(*args != nullptr) {
		while(**args == ' ')
			++*args;
	}

	return true;
}
