/*
 *  Advanced Programming Course Assignment 1
 *  Shell internal commands Implementation File
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

#include "../include/shell_internal_cmds.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

extern char *homedir;
extern char *cwd;
extern char *workingdir;
extern char *curr_prompt;

Result cmdCD(char *path, int argc) {
	// Only one argument is allowed, like in the original shell.
	if (argc > 2)
	{
		fprintf(stderr, "%s\n", SHELL_ERR_CMD_CD_ARG);
		return Failure;
	}

	else if (argc == 2)
	{
		// Home directory.
		if (strcmp(path, "~") == 0)
		{
			// Save current working directory before changing it.
			getcwd(workingdir, SHELL_MAX_PATH_LENGTH);

			if (chdir(homedir) == -1)
			{
				perror("Internal error: System call faliure: chdir(2)");
				return Failure;
			}

			return Success;
		}

		// Previous directory.
		else if (strcmp(path, "-") == 0)
		{
			if (strcmp(workingdir, "") == 0)
				return Success;

			if (chdir(workingdir) == -1)
			{
				perror("Internal error: System call faliure: chdir(2)");
				return Failure;
			}

			getcwd(workingdir, SHELL_MAX_PATH_LENGTH);

			return Success;
		}

		getcwd(workingdir, SHELL_MAX_PATH_LENGTH);

		if (chdir(path) == -1)
		{
			fprintf(stderr, "%s\n", SHELL_ERR_CMD_CD);
			return Failure;
		}
	}

	// No arguments - go to home directory.
	else
	{
		getcwd(workingdir, SHELL_MAX_PATH_LENGTH);

		if (chdir(homedir) == -1)
		{
			perror("Internal error: System call faliure: chdir(2)");
			return Failure;
		}
	}

	return Success;
}

Result cmdPWD() {
	fprintf(stdout, "%s\n", cwd);
	return Success;
}

Result cmdClear() {
	// Write the clear screen command to the standard output.
	write(STDOUT_FILENO, SHELL_CMD_CLEAR_FLUSH, SHELL_CMD_CLEAR_FLUSH_LEN);
	return Success;
}

Result cmdChangePrompt(char *new_prompt) {
	if (new_prompt == NULL)
	{
		fprintf(stderr, "%s\n", SHELL_ERR_CMD_CHANGE_PROMPT_SYNTAX);
		return Failure;
	}
	
	if (strlen(new_prompt) > SHELL_MAX_PATH_LENGTH)
	{
		fprintf(stderr, "%s\n", SHELL_ERR_CMD_CHANGE_PROMPT_LONG);
		return Failure;
	}
	
	strcpy(curr_prompt, new_prompt);
	return Success;
}

extern int last_status;  // Assuming you store the last command status in this global variable.
// @TODO: help from Roy how to handle it


Result cmdEcho(char **args, int argc) {
    bool redirect = false;
    FILE *fp = stdout;  // Default output to stdout.

    for (int i = 1; i < argc; i++) {
        if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0) {
            // Handle redirection
            redirect = true;
            const char *mode = (strcmp(args[i], ">") == 0) ? "w" : "a";
            if (++i < argc) {
                fp = fopen(args[i], mode);
                if (!fp) {
                    perror("fopen");
                    return Failure;
                }
            } else {
                fprintf(stderr, "Missing file name for redirection.\n");
                return Failure;
            }
            break;  // Stop processing further arguments after redirection.
        } else if (strcmp(args[i], "$?") == 0) {
            // Print the status of the last executed command.
            fprintf(fp, "%d ", last_status);
        } else {
            // Print the argument.
            fprintf(fp, "%s ", args[i]);
        }
    }

    if (!redirect) {
        fprintf(fp, "\n");  // Add a newline if outputting to stdout.
    } else if (fp != stdout) {
        fclose(fp);  // Close the file if we opened one for redirection.
    }

    return Success;
}
