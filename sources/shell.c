/*
 *  Advanced Programming Course Assignment 1
 *  Shell Program
 *  Copyright (C) 2023  Roy Simanovich and Almog Shor
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
#include <unistd.h>

int main(int argc, char** argv) {
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(*(argv + i), "-h") == 0 || strcmp(*(argv + i), "--help") == 0)
		{
			printf("Usage: %s [OPTION]...\n", *argv);
			printf("Run the shell program.\n\n");
			printf("  -h, --help\t\t\tDisplay this help and exit\n");
			printf("  -v, --version\t\t\tDisplay version information and exit\n");
			return EXIT_SUCCESS;
		}

		else if (strcmp(*(argv + i), "-v") == 0 || strcmp(*(argv + i), "--version") == 0)
		{
			printf("Shell Program 1.0\n");
			return EXIT_SUCCESS;
		}

		else
		{
			fprintf(stderr, "%s: unrecognized option '%s'\n", *argv, *(argv + i));
			fprintf(stderr, "Try '%s --help' for more information.\n", *argv);
			return EXIT_FAILURE;
		}
	}

	fprintf(stdout, "hello: ");
	fflush(stdout);

	return EXIT_SUCCESS;
}