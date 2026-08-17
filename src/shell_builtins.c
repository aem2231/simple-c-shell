#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/shell_builtins.h"

void
display_builtin_error()
{
	const char *err_info = "unknown error\n";
	switch (errno)
	{
	case EACCES:
		err_info = "Search permission is denied for one of the components of "
				   "path\n";
		break;
	case EFAULT:
		err_info = "path points outside accessible address space\n";
		break;
	case EIO:
		err_info = "An I/O error occurred\n";
		break;
	case ELOOP:
		err_info
			= "Too many symbolic links were encountered in resolving path\n";
		break;
	case ENAMETOOLONG:
		err_info = "path is too long\n";
		break;
	case ENOENT:
		err_info = "The directory specified in path does not exist\n";
		break;
	case ENOMEM:
		err_info = "Insufficient kernel memory was available\n";
		break;
	case ENOTDIR:
		err_info = "A component of path is not a directory\n";
		break;
	}

	printf("%s", err_info);
}

int
shell_cd(char **args)
{
	if (args[0] == NULL)
	{
		fprintf(stderr, "expected argument to \"cd\"\n");
	}
	else if (chdir(args[1]) != 0)
	{
		display_builtin_error();
		return -1;
	}

	return 1;
}

int
shell_exit(char **args)
{
	exit((long int)args);
}
