#include <ctype.h>
#include <errno.h>	// For error handling
#include <fcntl.h>	// For file control options used with open()
#include <signal.h> // For signal handling
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>	// For standard input/output functions like printf, stderr
#include <stdlib.h> // For memory allocation (malloc, free) and exit
#include <string.h> // For string manipulation functions like strcmp, strtok
#include <sys/types.h> // For data types like pid_t
#include <sys/wait.h>  // For waitpid and related macros
#include <unistd.h> // For POSIX operating system API, including fork, exec, pipe, chdir

#include "../include/shell_builtins.h"

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

const char *builtins[] = { "cd", "exit" };

char *
tokenise(char **cursor)
{

	bool skip_whitespace_fill;

	// skip leading whitespace
	while (isspace(**cursor) && **cursor != '\0')
		(*cursor)++;

	// already at the end of string - nothing but whitespace so return null
	if (**cursor == '\0')
		return NULL;

	char *start = *cursor; // need to track where we started

	// iterate through the tokens chars unitl we hit whitespace or null
	// terminator
	while ((!isspace(**cursor) && **cursor != '\0') && (**cursor != '\''))
	{
		printf("%c\n", **cursor);
		(*cursor)++;
	}

	if (**cursor == '\'')
	{
		(*cursor)++;
		while (**cursor != '\'' && **cursor != '\0')
		{
			printf("%c\n", **cursor);
			(*cursor)++;
		}
	}

	if (**cursor != '\0') // at the end of this token,
						  // so check if its /0
	{
		**cursor = '\0'; // and replace if it isnt
		(*cursor)++;	 // then go forwatd one to prevent bugs
	}

	return start;
}

char **
parse_args(char *command)
{
	char **args = malloc(MAX_ARGS * sizeof(char *));
	int i = 0;
	char *cursor = command; // cursor is a pointer to the current position,
							// needed by tokenise to know where we are
	char *token = tokenise(&cursor);
	while (token != NULL && i < (MAX_ARGS))
	{
		args[i++] = token;
		token = tokenise(&cursor);
	}
	args[i] = NULL;
	return args;
}

int
execute(char **args)
{
	// new child process
	int process_id = fork();
	int pid_status;

	if (process_id < 0)
	{ // fork always returns -1 on error
		printf("Fork failed\n");
	}
	else if (process_id == 0)
	{ // new process created
		if (execvp(args[0], args) == -1)
		{
			printf("Command not found: %s\n", args[0]);
			exit(-1);
		}
	}
	else
	{
		// wait for the child process created above to finish
		waitpid(process_id, &pid_status, 0);
	}

	return pid_status;
}

int
main()
{
	char line_buff[MAX_CMD_LEN];
	int status = 0;

	while (true)
	{ // REPL
		char cwd[200];
		getcwd(cwd, 200);
		printf("%s $ ", cwd);
		fflush(stdout); // force buffer to flush so prompt appears immediately
		// wait for input

		if (fgets(line_buff, sizeof(line_buff), stdin) == NULL)
			exit(0);

		char **parsed_args = parse_args(line_buff);

		if (parsed_args[0] != NULL)
		{
			if (strcmp(parsed_args[0], "cd") == 0)
				shell_cd(parsed_args);
			else if (strcmp(parsed_args[0], "exit") == 0)
				shell_exit(parsed_args);
			else
				status = execute(parsed_args);

			free(parsed_args);
		}
	}
}
