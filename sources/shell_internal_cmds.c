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