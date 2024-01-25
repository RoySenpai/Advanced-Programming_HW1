/*
 *  Advanced Programming Course Assignment 1
 *  Shell Program
 *  Copyright (C) 2024  Roy Simanovich and Almog Shor
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../include/shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <unistd.h>

// Global variables

// Home directory
char *homedir = NULL;

// Current working directory
char *cwd = NULL;

// Previous working directory
char *workingdir = NULL;

// Current prompt (default is SHELL_DEFAULT_PROMPT).
char *curr_prompt = NULL;

int main() {
	// Command buffer
	char command[SHELL_MAX_COMMAND_LENGTH + 1] = {0};

	// Arguments array
	char **argv = NULL;

	// Get home directory
	homedir = getenv("HOME");

	if (homedir == NULL)
	{
		fprintf(stderr,"Internal error: System call faliure: getenv(3)");
		return EXIT_FAILURE;
	}

	// Handle SIGINT. We use sigaction(2) instead of signal(2) because it's more portable and reliable.
	struct sigaction sa;
	sa.sa_handler = shell_sig_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	
	if(sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("Internal error: System call faliure: sigaction(2)");
		return EXIT_FAILURE;
	}

	// Allocate memory for the current working directory, the working directory and the current prompt.
	cwd = (char *)calloc((SHELL_MAX_PATH_LENGTH + 1), sizeof(char));
	workingdir = (char *)calloc((SHELL_MAX_PATH_LENGTH + 1), sizeof(char));
	curr_prompt = (char *)calloc((SHELL_MAX_PATH_LENGTH + 1), sizeof(char));

	if (cwd == NULL || workingdir == NULL || curr_prompt == NULL)
	{
		perror("Internal error: System call faliure: calloc(3)");
		return EXIT_FAILURE;
	}

	// Set the prompt to the default one.
	strcpy(curr_prompt, SHELL_DEFAULT_PROMPT);

	while (1)
	{
		// Get current working directory
		getcwd(cwd, SHELL_MAX_PATH_LENGTH);

		// Print prompt
		fprintf(stdout, "%s: ", curr_prompt);
		fflush(stdout);

		// Read command from user
		fgets(command, SHELL_MAX_COMMAND_LENGTH, stdin);

		// Pass command to shell internal parser
		if (parse_command(command, &argv) == Internal)
			continue;

		// Execute command
		//execute_command(argv);

		// Free the memory allocated for the arguments array.
		for (size_t k = 0; *(argv + k) != NULL; ++k)
			free(*(argv + k));

		free(argv);

		// Reset stdin buffer
		bzero(command, SHELL_MAX_PATH_LENGTH + 1);
	}

	// Memory cleanup
	shell_cleanup();

	return EXIT_SUCCESS;
}

void shell_sig_handler(int signum) {
	if (signum == SIGINT || signum == SIGQUIT)
	{
		fprintf(stdout, "\33[2K\rYou typed Control-C!\n");
		fflush(stdout);
	}
}

void shell_cleanup() {
	// Free the memory allocated for the current working directory.
	free(cwd);

	// Free the memory allocated for the working directory (privous directory, before changing to the current one).
	free(workingdir);

	// Free the memory allocated for the current prompt.
	free(curr_prompt);
}

CommandType parse_command(char *command, char ***argv) {
	char *token = NULL;
	int words = 1;
	bool inQuotes = false;

	// Remove the newline character from the command, to check if it's empty.
	command = strtok(command, "\n");

	// Safe-fail if the command is empty, to avoid segmentation fault.
	if (command == NULL || strlen(command) == 0)
		return Internal;

	// Count the number of words (with support for quoted arguments).
	for (size_t k = 0; k < strlen(command); ++k)
	{
		if (*(command + k) == '"')
			inQuotes = !inQuotes;

		if (!inQuotes && *(command + k) == ' ' && *(command + k + 1) != ' ')
			++words;
	}

	// Parse the first argument (the command itself).
	token = strtok(command, " ");

	// Exit command.
	if (strcmp(token, SHELL_CMD_EXIT) == 0)
	{
		shell_cleanup();
		exit(EXIT_SUCCESS);
	}

	// Change directory command.
	else if (strcmp(token, SHELL_CMD_CD) == 0)
	{
		// Get the next token, which is the directory to change to.
		token = strtok(NULL, " ");
		cmdCD(token, words);
		return Internal;
	}

	// Clean screen command.
	else if (strcmp(token, SHELL_CMD_CLEAR) == 0)
	{
		cmdClear();
		return Internal;
	}

	// Print working directory command.
	else if (strcmp(token, SHELL_CMD_PWD) == 0)
	{
		cmdPWD();
		return Internal;
	}

	// Change prompt command.
	else if (strcmp(token, SHELL_CMD_CHANGE_PROMPT) == 0)
	{
		// Check if the number of arguments is correct.
		if (words != 3)
		{
			fprintf(stderr, "%s\n", SHELL_ERR_CMD_CHANGE_PROMPT_SYNTAX);
			return Internal;
		}

		token = strtok(NULL, " ");

		// Check if the syntax is correct.
		if (strcmp(token, "=") != 0)
		{
			fprintf(stderr, "%s\n", SHELL_ERR_CMD_CHANGE_PROMPT_SYNTAX);
			return Internal;
		}

		// Extract the new prompt and pass it to the command.
		token = strtok(NULL, " ");

		cmdChangePrompt(token);
		return Internal;
	}

	// Allocate memory for the arguments array.
	*argv = (char **)calloc((words + 1), sizeof(char *));
	char **pargv = *argv;

	if (*argv == NULL)
	{
		perror("Internal error: System call faliure: calloc(3)");
		shell_cleanup();
		exit(EXIT_FAILURE);
	}

	// Parse the arguments (if there are any).
	while (token != NULL)
	{
		// Allocate memory for the argument.
		*pargv = (char *)calloc((strlen(token) + 1), sizeof(char));

		if (*pargv == NULL)
		{
			perror("Internal error: System call faliure: calloc(3)");
			free(*argv);
			return Internal;
		}

		
		// If the argument is quoted, we need to parse it differently.
		// TODO: Fix this. Does SIGSEGV when using quoted arguments.
		/*
		if (*token == '"' && strlen(token) > 1)
		{
			strcpy(*pargv, (token + 1));

			while (token != NULL)
			{
				token = strtok(NULL, "\"");

				// Reallocate memory for the argument.
				*pargv = realloc(*pargv, (strlen(*pargv) + strlen(token) + 2) * sizeof(char));

				// Zero the memory allocated for the rest of the argument.
				memset(*pargv + strlen(*pargv), '\0', strlen(token) + 2);

				if (token != NULL)
				{
					strcat(*pargv, " ");
					strcat(*pargv, token);
				}
			}

			if (*(*pargv + strlen(*pargv) - 1) == '"')
				*(*pargv + strlen(*pargv) - 1) = '\0';

			++pargv;
			token = strtok(NULL, " ");

			continue;
		}*/

		strcpy(*pargv, token);
		++pargv;
		token = strtok(NULL, " ");
	}

	// Set the last argument to NULL, as required by execvp.
	// By definition, the last argument must be NULL, as we allocate just enough memory for the arguments.
	// We also make sure to free the memory allocated for the last argument, to avoid memory leaks.
	if (*pargv != NULL)
	{
		free(*pargv);
		*pargv = NULL;
	}

	// This is an external command.
	return External;
}