/*
 *  Advanced Programming Course Assignment 1
 *  Shell Command Implementation File
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

#include "../include/Command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PCommand create_command(char *command, bool isInternal, bool background) {
    if (command == NULL)
    {
        fprintf(stderr, "Error: create_command() failed: command is NULL\n");
        return NULL;
    }

    PCommand cmd = (PCommand)malloc(sizeof(Command));

    if (cmd == NULL)
    {
        perror("Error: create_command() failed: malloc() failed");
        return NULL;
    }

    cmd->command = (char *)malloc(strlen(command) + 1);

    if (cmd->command == NULL)
    {
        perror("Error: create_command() failed: malloc() failed");
        free(cmd);
        return NULL;
    }

    strcpy(cmd->command, command);

    cmd->isInternal = isInternal;
    cmd->background = background;
    cmd->status = 0;

    return cmd;
}

void destroy_command(PCommand cmd) {
    if (cmd == NULL)
    {
        fprintf(stderr, "Error: destroy_command() failed: cmd is NULL\n");
        return;
    }

    free(cmd->command);
    free(cmd);
}