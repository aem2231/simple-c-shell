#include <errno.h>	// For error handling
#include <fcntl.h>	// For file control options used with open()
#include <signal.h> // For signal handling
#include <stdio.h>	// For standard input/output functions like printf, stderr
#include <stdlib.h> // For memory allocation (malloc, free) and exit
#include <string.h> // For string manipulation functions like strcmp, strtok
#include <sys/types.h> // For data types like pid_t
#include <sys/wait.h>  // For waitpid and related macros
#include <unistd.h> // For POSIX operating system API, including fork, exec, pipe, chdir

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

char **
parse (char *command)
{
	char **args = malloc (MAX_ARGS * sizeof (char *));
	int i = 0;

	char *token = strtok (command, " \t\n");
	while (token != NULL) // iterate through tokens until there is none left
	{
		args[i++] = token;				// and add the tokens to our args
		token = strtok (NULL, " \t\n"); // update token to store the next token
	}
	args[i] = NULL; // NULL terminate  arr

	return args;
}

pid_t
execute (char **args)
{
	// new child process
	int process_id = fork ();

	if (process_id < 0) // fork always returns -1 on error
	{
		printf ("Fork failed\n");
	}
	else if (process_id == 0) // new process created
	{
		if (execvp (args[0], args) == -1)
			printf ("Command not found.\n");
	}
	else
	{
		// wait for the child process created above to finish
		wait (NULL);
	}

	return process_id;
}

int
main ()
{
	char line_buff[MAX_CMD_LEN];

	while (1) // REPL (Read, Eval, Print, Loop). Each iteration = one command
	{
		printf ("$ ");
		fflush (stdout); // force buffer to flush so prompt appears immediately
		// wait for input
		if (!fgets (line_buff, sizeof (line_buff), stdin))
			break;

		char **parsed_args = parse (line_buff);

		if (parsed_args[0] != NULL)
		{
			pid_t pid = execute (parsed_args);
		}

		free (parsed_args);
	}
}
