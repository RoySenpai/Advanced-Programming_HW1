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
#include <string.h>
#include <signal.h>
#include <unistd.h>

char *homedir = NULL;
char *cwd = NULL;
char *workingdir = NULL;
char *curr_prompt = NULL;

int main() {
	// Get home directory
	homedir = getenv("HOME");

	if (homedir == NULL)
	{
		fprintf(stderr,"Internal error: System call faliure: getenv(3)");
		return EXIT_FAILURE;
	}

	// Handle SIGINT
	signal(SIGINT, shell_sig_handler);

	// Allocate memory
	cwd = (char *)calloc((SHELL_MAX_PATH_LENGTH + 1), sizeof(char));
	workingdir = (char *)calloc((SHELL_MAX_PATH_LENGTH + 1), sizeof(char));
	curr_prompt = (char *)calloc((SHELL_MAX_PATH_LENGTH + 1), sizeof(char));

	if (cwd == NULL || workingdir == NULL || curr_prompt == NULL)
	{
		perror("Internal error: System call faliure: calloc(3)");
		return EXIT_FAILURE;
	}

	strcpy(curr_prompt, SHELL_DEFAULT_PROMPT);

	while (1)
	{
		fprintf(stdout, "%s: ", curr_prompt);
		fflush(stdout);
	}

	// Memory cleanup
	free(cwd);
	free(workingdir);
	free(curr_prompt);

	return EXIT_SUCCESS;
}

void shell_sig_handler(int signum) {
	if (signum == SIGINT)
	{
		fprintf(stdout, "You typed Control-C!\n");
		fprintf(stdout, "%s: ", curr_prompt);
		fflush(stdout);
	}
}