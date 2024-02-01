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
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

// Global variables section

// Home directory
char *homedir = NULL;

// Current working directory
char *cwd = NULL;

// Previous working directory
char *workingdir = NULL;

// Current prompt (default is SHELL_DEFAULT_PROMPT).
char *curr_prompt = NULL;

// Command history
PLinkedList commandHistory;

// Variable list
PLinkedList variableList;

// Shell state
State shell_state = STATE_NETURAL;

// Main function section
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
	// Size is SHELL_MAX_PATH_LENGTH + 1 to allow for the null terminator.
	cwd = (char *)calloc((SHELL_MAX_PATH_LENGTH + 1), sizeof(char));
	workingdir = (char *)calloc((SHELL_MAX_PATH_LENGTH + 1), sizeof(char));
	curr_prompt = (char *)calloc((SHELL_MAX_PATH_LENGTH + 1), sizeof(char));

	if (cwd == NULL || workingdir == NULL || curr_prompt == NULL)
	{
		perror("Internal error: System call faliure: calloc(3)");
		return EXIT_FAILURE;
	}

	commandHistory = createLinkedList();
	variableList = createLinkedList();

	if (commandHistory == NULL || variableList == NULL)
	{
		shell_cleanup();
		exit(EXIT_FAILURE);
	}

	// Add the last status variable to the variable list.
	if (setVariable(SHELL_CMD_LAST_STATUS, "0") == Failure)
	{
		shell_cleanup();
		exit(EXIT_FAILURE);
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
		execute_command(argv);

		// Free the memory allocated for the arguments array.
		char **tmp = argv;

		while (*tmp != NULL)
		{
			free(*tmp);
			++tmp;
		}

		free(argv);

		// Reset stdin buffer
		bzero(command, SHELL_MAX_PATH_LENGTH + 1);
	}

	// Memory cleanup
	shell_cleanup();

	return EXIT_SUCCESS;
}

void shell_sig_handler(int signum) {
	if (signum == SIGINT)
	{
		fprintf(stdout, "\33[2K\rYou typed Control-C!\n");
		fflush(stdout);
	}
}

void update_laststatus(int status) {
	// Set the last status variable.
	char status_str[10] = {0};
	sprintf(status_str, "%d", status);
	setVariable(SHELL_CMD_LAST_STATUS, status_str);
}

void shell_cleanup() {
	// Free the memory allocated for the current working directory.
	free(cwd);

	// Free the memory allocated for the working directory (privous directory, before changing to the current one).
	free(workingdir);

	// Free the memory allocated for the current prompt.
	free(curr_prompt);

	// Free the memory allocated for the command history.
	PNode curr = commandHistory->head;

	while (curr != NULL)
	{
		PNode tmp = curr;
		curr = curr->next;
		
		PCommand command = (PCommand)(tmp->data);
		free(command->command);
		free(command);
		free(tmp);
	}

	commandHistory->head = NULL;

	destroyLinkedList(commandHistory);

	// Free the memory allocated for the variable list.
	curr = variableList->head;

	while (curr != NULL)
	{
		PNode tmp = curr;
		curr = curr->next;
		
		PVariable variable = (PVariable)(tmp->data);
		free(variable->name);
		free(variable->value);
		free(variable);
		free(tmp);
	}

	variableList->head = NULL;

	destroyLinkedList(variableList);
}

CommandType parse_command(char *command, char ***argv) {
	int words = 1;
	bool inQuotes = false;
	Result curr_res = Success;

	// Remove the newline character from the command, to check if it's empty.
	command = strtok(command, "\n");

	// Safe-fail if the command is empty or only contains & (illegal), to avoid segmentation fault.
	if (command == NULL || strlen(command) == 0 || strcmp(command, "&") == 0)
		return Internal;

	if (strcmp(command, SHELL_CMD_REPEATED) == 0)
	{
		cmdrepeatLastCommand();
		return Internal;
	}

	// Count the number of words (with support for quoted arguments).
	for (size_t k = 0; k < strlen(command); ++k)
	{
		if (*(command + k) == '"')
			inQuotes = !inQuotes;

		if (!inQuotes && *(command + k) == ' ' && *(command + k + 1) != ' ')
			++words;
	}

	// Tokenize the command into an array of arguments.
	*argv = tokenize_command(command, words);
	char **pargv = *argv;

	// If the tokenization failed, state an error and exit.
	if (pargv == NULL)
	{
		shell_cleanup();
		exit(EXIT_FAILURE);
	}

	// Parse the variables.
	parse_variables(&pargv, variableList);

	if (is_control_command(*pargv))
	{
		// If control command, check if it's valid.
		if (strcmp(*pargv, "if") == 0)
		{
			if (shell_state != STATE_NETURAL)
			{
				fprintf(stderr, "Shell internal error: syntax error: if unexpected\n");
				return Internal;
			}

			shell_state = STATE_WANT_THEN;

			// Free the first argument, which is the if command.
			free(*pargv);

			// Shift the arguments array to the left by one.
			char **tmp = pargv;
			while (*tmp != NULL)
			{
				*tmp = *(tmp + 1);
				++tmp;
			}

			// Free the last argument, which is now NULL.
			free(*(pargv + words - 1));
			*(pargv + words - 1) = NULL;
		}

		// Then command must be after if command.
		else if (strcmp(*pargv, "then") == 0)
		{
			if (words > 1 || shell_state != STATE_WANT_THEN)
			{
				fprintf(stderr, "Shell internal error: syntax error: then unexpected\n");
				return Internal;
			}

			shell_state = STATE_THEN_BLOCK;

			return Internal;
		}

		// Else command must be after the then command block.
		else if (strcmp(*pargv, "else") == 0)
		{
			if (words > 1 || shell_state != STATE_THEN_BLOCK)
			{
				fprintf(stderr, "Shell internal error: syntax error: else unexpected\n");
				return Internal;
			}

			shell_state = STATE_ELSE_BLOCK;

			return Internal;
		}

		// Finish control command. Reset shell state.
		else if (strcmp(*pargv, "fi") == 0)
		{
			if (words > 1 || (shell_state != STATE_THEN_BLOCK && shell_state != STATE_ELSE_BLOCK))
			{
				fprintf(stderr, "Shell internal error: syntax error: fi unexpected\n");
				return Internal;
			}

			shell_state = STATE_NETURAL;

			return Internal;
		}
	}

	// We want to check if its ok to execute the command if we are in then or else block.
	if (commandHistory->tail != NULL && shell_state != STATE_WANT_THEN && shell_state != STATE_NETURAL)
	{
		PCommand lastCommand = (PCommand)(commandHistory->tail->data);

		curr_res = (lastCommand->status ? Failure:Success);

		if (!ok_to_execute(shell_state, curr_res))
			return Internal;
	}

	// If we expect "then" and we got a command, it's a syntax error.
	else if (shell_state == STATE_WANT_THEN && strncmp(command, "if", 2) != 0)
	{
		fprintf(stderr, "Shell internal error: syntax error: then expected\n");
		return Internal;
	}

	// Add the command to the command history.
	PCommand cmd = create_command(command, false, false);

	if (addNode(commandHistory, cmd) != 0)
	{
		destroy_command(cmd);

		for (size_t k = 0; *(pargv + k) != NULL; ++k)
			free(*(pargv + k));

		free(pargv);
		
		shell_cleanup();
		exit(EXIT_FAILURE);
	}

	// Exit command.
	if (strcmp(*pargv, SHELL_CMD_EXIT) == 0)
	{
		// Clean up arguments array.
		for (size_t k = 0; *(pargv + k) != NULL; ++k)
			free(*(pargv + k));

		free(pargv);
		shell_cleanup();
		exit(EXIT_SUCCESS);
	}

	// Change directory command.
	else if (strcmp(*pargv, SHELL_CMD_CD) == 0)
	{
		Result res = cmdCD(*(pargv + 1), words);
		cmd->isInternal = true;
		cmd->status = (res == Success) ? 0 : 1;
		update_laststatus(cmd->status);
		return Internal;
	}

	// Clean screen command.
	else if (strcmp(*pargv, SHELL_CMD_CLEAR) == 0)
	{
		cmdClear();
		cmd->isInternal = true;
		cmd->status = 0;
		update_laststatus(0);
		return Internal;
	}

	// Print working directory command.
	else if (strcmp(*pargv, SHELL_CMD_PWD) == 0)
	{
		cmdPWD();
		cmd->isInternal = true;
		cmd->status = 0;
		update_laststatus(0);
		return Internal;
	}

	// Change prompt command.
	else if (strcmp(*pargv, SHELL_CMD_CHANGE_PROMPT) == 0)
	{
		cmd->isInternal = true;

		// Check if the number of arguments is correct.
		if (words != 3)
		{
			fprintf(stderr, "%s\n", SHELL_ERR_CMD_CHANGE_PROMPT_SYNTAX);
			cmd->status = 1;
			update_laststatus(1);
			return Internal;
		}

		// Check if the syntax is correct.
		if (strcmp(*(pargv + 1), "=") != 0)
		{
			fprintf(stderr, "%s\n", SHELL_ERR_CMD_CHANGE_PROMPT_SYNTAX);
			cmd->status = 1;
			update_laststatus(1);
			return Internal;
		}

		Result res = cmdChangePrompt(*(pargv + 2));
		cmd->status = (res == Success) ? 0 : 1;
		update_laststatus(cmd->status);
		return Internal;
	}

	// History command.
	else if (strcmp(*pargv, SHELL_CMD_HISTORY) == 0)
	{
		cmd->isInternal = true;
		Result res = cmdHistory(words);
		cmd->status = (res == Success) ? 0 : 1;
		update_laststatus(cmd->status);
		return Internal;
	}

	// Read command.
	else if (strcmp(*pargv, SHELL_CMD_READ) == 0)
	{
		cmd->isInternal = true;
		Result res = cmdRead(*(pargv + 1));
		cmd->status = (res == Success) ? 0 : 1;
		update_laststatus(cmd->status);
		return Internal;
	}

    // !! command. cmdrepeatLastCommand
    else if (strcmp(*pargv, "!!") == 0) {
        cmd->isInternal = true;
        Result res = cmdrepeatLastCommand();
        cmd->status = (res == Success) ? 0 : 1;
        update_laststatus(cmd->status);
        return Internal;
    }

    // Set Variable command.
	else if (*command == '$')
	{
		cmd->isInternal = true;

		if (words != 3)
		{
			fprintf(stderr, "%s\n", SHELL_ERR_CMD_SET_SYNTAX);
			cmd->status = 1;
			update_laststatus(1);
			return Internal;
		}

		else if (strcmp(*(pargv + 1), "=") != 0)
		{
			fprintf(stderr, "%s\n", SHELL_ERR_CMD_SET_SYNTAX);
			cmd->status = 1;
			update_laststatus(1);
			return Internal;
		}

		Result res = setVariable(*(pargv + 0) + 1, *(pargv + 2));
		cmd->status = (res == Success) ? 0 : 1;
		update_laststatus(cmd->status);
		return Internal;
	}

	// Check if the command is a background command.
	if (*(command + strlen(command) - 1) == '&')
		cmd->background = true;

	// This is an external command.
	return External;
}

void execute_command(char **argv) {
	pid_t pid;
	int status = 0, i = 0, num_pipes = 0;
	int count_args = 0;
	bool redirect = false;
	char ***pipes = NULL;

	// Count the number of pipes and check if there are any redirections.
	while (*(argv + i) != NULL)
	{
		if (strcmp(*(argv + i), "|") == 0)
			++num_pipes;

		else if (strcmp(*(argv + i), ">") == 0 || 
				strcmp(*(argv + i), ">>") == 0 || 
				strcmp(*(argv + i), "<") == 0 || 
				strcmp(*(argv + i), "2>") == 0)
			redirect = true;

		++i;
		++count_args;
	}

	// Case 1: No pipes
	if (num_pipes == 0)
	{
		// Fork the process.
		pid = fork();

		if (pid == -1)
		{
			perror("Internal error: System call faliure: fork(2)");
			shell_cleanup();
			exit(EXIT_FAILURE);
		}

		// Child process.
		else if (pid == 0)
		{
			// Reset SIGINT to default.
			signal(SIGINT, SIG_DFL);

			// Handle redirects
			if (redirect)
			{
				bool in_mode = false, out_mode = false, err_mode = false;
				int input_fd = STDIN_FILENO, output_fd = STDOUT_FILENO, append_fd = STDOUT_FILENO, error_fd = STDERR_FILENO;

				for (int i = 0; i < count_args && !(in_mode && out_mode && err_mode); ++i)
				{
					if (strcmp(*(argv + i), "<") == 0)
					{
						if (in_mode)
						{
							fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_IN_TWICE);
							exit(EXIT_FAILURE);
						}

						else if (*(argv + i + 1) == NULL)
						{
							fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_NO_FILE);
							exit(EXIT_FAILURE);
						}

						// Open the file.
						input_fd = open(*(argv + i + 1), O_RDONLY);

						if (input_fd == -1)
						{
							perror("Internal error: System call faliure: open(2)");
							exit(EXIT_FAILURE);
						}

						// Redirect stdin to the file.
						dup2(input_fd, STDIN_FILENO);

						// Close the file.
						close(input_fd);

						in_mode = true;

						// Remove the redirection arguments from the arguments array.
						free(*(argv + i));
						*(argv + i) = NULL;
					}

					else if (strcmp(*(argv + i), ">") == 0)
					{
						if (out_mode)
						{
							fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_OUT_TWICE);
							exit(EXIT_FAILURE);
						}

						else if (*(argv + i + 1) == NULL)
						{
							fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_NO_FILE);
							exit(EXIT_FAILURE);
						}

						// Open the file.
						output_fd = open(*(argv + i + 1), O_WRONLY | O_CREAT | O_TRUNC, 0644);

						if (output_fd == -1)
						{
							perror("Internal error: System call faliure: open(2)");
							exit(EXIT_FAILURE);
						}

						// Redirect stdout to the file.
						dup2(output_fd, STDOUT_FILENO);

						// Close the file.
						close(output_fd);

						out_mode = true;

						// Remove the redirection arguments from the arguments array.
						free(*(argv + i));
						*(argv + i) = NULL;
					}

					else if (strcmp(*(argv + i), ">>") == 0)
					{
						if (out_mode)
						{
							fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_OUT_TWICE);
							exit(EXIT_FAILURE);
						}

						else if (*(argv + i + 1) == NULL)
						{
							fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_NO_FILE);
							exit(EXIT_FAILURE);
						}

						// Open the file.
						append_fd = open(*(argv + i + 1), O_WRONLY | O_CREAT | O_APPEND, 0644);

						if (append_fd == -1)
						{
							perror("Internal error: System call faliure: open(2)");
							exit(EXIT_FAILURE);
						}

						// Redirect stdout to the file.

						dup2(append_fd, STDOUT_FILENO);

						// Close the file.
						close(append_fd);

						out_mode = true;

						// Remove the redirection arguments from the arguments array.
						free(*(argv + i));
						*(argv + i) = NULL;
					}

					else if (strcmp(*(argv + i), "2>") == 0)
					{
						if (err_mode)
						{
							fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_OUT_TWICE);
							exit(EXIT_FAILURE);
						}

						else if (*(argv + i + 1) == NULL)
						{
							fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_NO_FILE);
							exit(EXIT_FAILURE);
						}

						// Open the file.
						error_fd = open(*(argv + i + 1), O_WRONLY | O_CREAT | O_TRUNC, 0644);

						if (error_fd == -1)
						{
							perror("Internal error: System call faliure: open(2)");
							exit(EXIT_FAILURE);
						}

						// Redirect stderr to the file.
						dup2(error_fd, STDERR_FILENO);

						// Close the file.
						close(error_fd);

						err_mode = true;

						// Remove the redirection arguments from the arguments array.
						free(*(argv + i));
						*(argv + i) = NULL;
					}
				}
			}

			// Execute the command.
			if (execvp(*argv, argv) == -1)
			{
				perror("Internal error: System call faliure: execvp(3)");
				shell_cleanup();
				exit(EXIT_FAILURE);
			}
		}

		// Parent process.
		else
		{
			PCommand cmd = (PCommand)(commandHistory->tail->data);

			if (cmd->background)
			{
				waitpid(pid, &status, WNOHANG);
				fprintf(stdout, "[%d]\n", pid);

				// Update the command history.
				cmd->status = status >> 8;

				// Set the last status variable.
				update_laststatus(cmd->status);

				return;
			}

			// Wait for the child process to finish.
			wait(&status);

			int high_8, bit_7;
			high_8 = status >> 8;
			bit_7 = status & 0x80;

			// Update the command history.
			cmd->status = (high_8 == 0 && bit_7 == 0) ? 0 : high_8;

			// Set the last status variable.
			update_laststatus(cmd->status);
		}

		return;
	}

	pipes = (char ***)calloc(num_pipes + 1, sizeof(char **));

	if (pipes == NULL)
	{
		perror("Internal error: System call faliure: calloc(3)");
		shell_cleanup();
		exit(EXIT_FAILURE);
	}

	// Allocate memory for each pipe.
	for (int k = 0; k < (num_pipes + 1); ++k)
	{
		*(pipes + k) = (char **)calloc(count_args, sizeof(char *));

		if (*(pipes + k) == NULL)
		{
			perror("Internal error: System call faliure: calloc(3)");

			for (int k = 0; k < num_pipes + 1; ++k)
			{
				if (*(pipes + k) != NULL)
					free(*(pipes + k));
			}

			free(pipes);

			shell_cleanup();
			exit(EXIT_FAILURE);
		}
	}

	// Parse the command into pipes.
	int arg_pos = 0, pipe_pos = 0;

	for (int i = 0; i < count_args; ++i)
	{
		if (strcmp(*(argv + i), "|") == 0)
		{
			++pipe_pos;
			arg_pos = 0;
			continue;
		}

		*(*(pipes + pipe_pos) + arg_pos) = *(argv + i);
		arg_pos++;
	}

	// Create pipes.
	int pipe_fds[num_pipes * 2];

	for (int k = 0; k < num_pipes; ++k)
	{
		if (pipe(pipe_fds + (k * 2)) == -1)
		{
			perror("Internal error: System call faliure: pipe(2)");

			for (int k = 0; k < num_pipes + 1; ++k)
				free(*(pipes + k));

			free(pipes);

			shell_cleanup();
			exit(EXIT_FAILURE);
		}
	}

	// Pipe position offset.
	int pipe_pos_h = 0;

	// Start the chain reaction of the pipes.
	for (int k = 0; k < num_pipes + 1; ++k)
	{
		// Fork the process.
		pid = fork();

		if (pid == -1)
		{
			perror("Internal error: System call faliure: fork(2)");
			for (int k = 0; k < num_pipes + 1; ++k)
				free(*(pipes + k));

			free(pipes);
			exit(EXIT_FAILURE);
		}

		else if (pid == 0)
		{
			char **curr_pipe = *(pipes + k);

			// Redirect in pipes handles.
			bool in_mode = false, out_mode = false, err_mode = false;
			int input_fd = STDIN_FILENO, output_fd = STDOUT_FILENO, append_fd = STDOUT_FILENO, error_fd = STDERR_FILENO;

			// Reset SIGINT to default.
			signal(SIGINT, SIG_DFL);
			
			// If not last pipe, redirect stdout to the next pipe.
			if (k < num_pipes)
				dup2(pipe_fds[pipe_pos_h + 1], STDOUT_FILENO);

			// If not first pipe, redirect stdin to the previous pipe.
			if (pipe_pos_h != 0)
				dup2(pipe_fds[pipe_pos_h-2], STDIN_FILENO);

			// Close all unnecessary pipe handles.
			for (int j = 0; j < num_pipes * 2; ++j)
				close(pipe_fds[j]);

			// Handle redirects
			if (redirect)
			{
				int count_args_in_pipe = 0;

				// Count the number of arguments in the current command.
				for (size_t k = 0; *(curr_pipe + count_args_in_pipe) != NULL; ++k)
					++count_args_in_pipe;

				// Allow only input redirection for the first command.
				if (k == 0)
				{
					for (int i = 0; i < count_args_in_pipe; ++i)
					{
						if (strcmp(*(curr_pipe + i), "<") == 0)
						{
							if (in_mode)
							{
								fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_IN_TWICE);
								exit(EXIT_FAILURE);
							}

							else if (*(curr_pipe + i + 1) == NULL)
							{
								fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_NO_FILE);
								exit(EXIT_FAILURE);
							}

							// Open the file.
							input_fd = open(*(curr_pipe + i + 1), O_RDONLY);

							if (input_fd == -1)
							{
								perror("Internal error: System call faliure: open(2)");
								exit(EXIT_FAILURE);
							}

							// Redirect stdin to the file.
							dup2(input_fd, STDIN_FILENO);

							// Close the file.
							close(input_fd);

							in_mode = true;

							// Remove the redirection arguments from the arguments array.
							free(*(curr_pipe + i));
							*(*(pipes + k) + i) = NULL;
						}

						else if (strcmp(*(curr_pipe + i), ">") == 0 || 
								strcmp(*(curr_pipe + i), ">>") == 0 || 
								strcmp(*(curr_pipe + i), "2>") == 0)
						{
							fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_OUT_IN_FIRST_PIPE);
							exit(EXIT_FAILURE);
						}
					}
				}

				// Allow only output redirection for the last command.
				else if (k == num_pipes)
				{
					for (int i = 0; i < count_args_in_pipe; ++i)
					{
						if (strcmp(*(curr_pipe + i), "<") == 0)
						{
							fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_IN_IN_LAST_PIPE);
							exit(EXIT_FAILURE);
						}

						else if (strcmp(*(curr_pipe + i), ">") == 0)
						{
							if (out_mode)
							{
								fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_OUT_TWICE);
								exit(EXIT_FAILURE);
							}

							else if (*(curr_pipe + i + 1) == NULL)
							{
								fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_NO_FILE);
								exit(EXIT_FAILURE);
							}

							// Open the file.
							output_fd = open(*(curr_pipe + i + 1), O_WRONLY | O_CREAT | O_TRUNC, 0644);

							if (output_fd == -1)
							{
								perror("Internal error: System call faliure: open(2)");
								exit(EXIT_FAILURE);
							}

							// Redirect stdout to the file.
							dup2(output_fd, STDOUT_FILENO);

							// Close the file.
							close(output_fd);

							out_mode = true;

							// Remove the redirection arguments from the arguments array.
							free(*(curr_pipe + i));
							*(curr_pipe + i) = NULL;
						}

						else if (strcmp(*(curr_pipe + i), ">>") == 0)
						{
							if (out_mode)
							{
								fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_OUT_TWICE);
								exit(EXIT_FAILURE);
							}

							else if (*(curr_pipe + i + 1) == NULL)
							{
								fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_NO_FILE);
								exit(EXIT_FAILURE);
							}

							// Open the file.
							append_fd = open(*(curr_pipe + i + 1), O_WRONLY | O_CREAT | O_APPEND, 0644);

							if (append_fd == -1)
							{
								perror("Internal error: System call faliure: open(2)");
								exit(EXIT_FAILURE);
							}

							// Redirect stdout to the file.
							dup2(append_fd, STDOUT_FILENO);

							// Close the file.
							close(append_fd);

							out_mode = true;

							// Remove the redirection arguments from the arguments array.
							free(*(curr_pipe + i));
							*(curr_pipe + i) = NULL;
						}

						else if (strcmp(*(curr_pipe + i), "2>") == 0)
						{
							if (err_mode)
							{
								fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_OUT_TWICE);
								exit(EXIT_FAILURE);
							}

							else if (*(curr_pipe + i + 1) == NULL)
							{
								fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_NO_FILE);
								exit(EXIT_FAILURE);
							}

							// Open the file.
							error_fd = open(*(curr_pipe + i + 1), O_WRONLY | O_CREAT | O_TRUNC, 0644);

							if (error_fd == -1)
							{
								perror("Internal error: System call faliure: open(2)");
								exit(EXIT_FAILURE);
							}

							// Redirect stderr to the file.
							dup2(error_fd, STDERR_FILENO);

							// Close the file.
							close(error_fd);

							err_mode = true;

							// Remove the redirection arguments from the arguments array.
							free(*(curr_pipe + i));
							*(curr_pipe + i) = NULL;
						}
					}
				}

				// Don't allow any redirections for the middle commands.
				else
				{
					for (int i = 0; i < count_args_in_pipe; ++i)
					{
						if (strcmp(*(curr_pipe + i), "<") == 0 || 
							strcmp(*(curr_pipe + i), ">") == 0 || 
							strcmp(*(curr_pipe + i), ">>") == 0 || 
							strcmp(*(curr_pipe + i), "2>") == 0)
						{
							fprintf(stderr, "%s\n", SHELL_ERR_REDIRECT_BETWEEN_PIPES);
							exit(EXIT_FAILURE);
						}
					}
				}
			}

			// Execute the command.
			if (execvp(*curr_pipe, curr_pipe) == -1)
			{
				perror("Internal error: System call faliure: execvp(3)");
				exit(EXIT_FAILURE);
			}
		}

		pipe_pos_h += 2;
	}

	// Close all pipe handles.
	for (int i = 0; i < num_pipes * 2; ++i)
		close(pipe_fds[i]);

	PCommand cmd = (PCommand)(commandHistory->tail->data);

	// If the command is a background command, print the process ID and return, don't wait for the child process to finish.
	if (cmd->background)
	{
		waitpid(pid, &status, WNOHANG);
		fprintf(stdout, "[%d]\n", pid);

		// Update the command history.
		cmd->status = status >> 8;

		// Set the last status variable.
		update_laststatus(cmd->status);

		return;
	}

	// Wait for all child processes to finish.
	for (int i = 0; i < num_pipes + 1; ++i)
		wait(&status);

	int high_8, bit_7;
	high_8 = status >> 8;
	bit_7 = status & 0x80;

	// Update the command history.
	cmd->status = (high_8 == 0 && bit_7 == 0) ? 0 : high_8;

	// Set the last status variable.
	update_laststatus(cmd->status);

	// Free the memory allocated for the pipes array.
	for (int k = 0; k < num_pipes + 1; ++k)
		free(*(pipes + k));

	free(pipes);
}