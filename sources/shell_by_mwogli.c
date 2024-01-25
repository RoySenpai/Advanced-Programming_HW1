#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_LENGTH 1024
#define DELIMITERS " \t\r\n\a"

// Function declarations
void loop();
char *read_line();
char **split_line(char *line);
int execute(char **args);
int execute_builtin(char **args);
int change_directory(char **args);
int launch(char **args);

// Global variables
char *prompt = "hello: ";

int main(int argc, char **argv)
{
	// Main shell loop
	loop();

	return EXIT_SUCCESS;
}

void loop()
{
	char *line;
	char **args;
	int status;

	do
	{
		printf("%s", prompt);
		line = read_line();
		args = split_line(line);
		status = execute(args);

		free(line);
		free(args);
	} while (status);
}

char *read_line()
{
	char *line = NULL;
	ssize_t bufsize = 0; // getline will allocate a buffer

	if (getline(&line, &bufsize, stdin) == -1)
	{
		if (feof(stdin))
		{
			exit(EXIT_SUCCESS); // We received an EOF
		}
		else
		{
			perror("readline");
			exit(EXIT_FAILURE);
		}
	}

	return line;
}

char **split_line(char *line)
{
	int bufsize = 64, position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		fprintf(stderr, "allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, DELIMITERS);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= bufsize)
		{
			bufsize += 64;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				fprintf(stderr, "allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, DELIMITERS);
	}
	tokens[position] = NULL;
	return tokens;
}

int execute(char **args)
{
	if (args[0] == NULL)
	{
		// An empty command was entered.
		return 1;
	}

	if (strcmp(args[0], "cd") == 0)
	{
		return change_directory(args);
	}

	// Add more built-in functions here

	return launch(args);
}

int change_directory(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "expected argument to \"cd\"\n");
	}
	else
	{
		if (chdir(args[1]) != 0)
		{
			perror("cd");
		}
	}
	return 1;
}

int launch(char **args)
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		// Child process
		if (execvp(args[0], args) == -1)
		{
			perror("shell");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		// Error forking
		perror("shell");
	}
	else
	{
		// Parent process
		do
		{
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}
