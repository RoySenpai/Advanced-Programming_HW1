/*
 *  Advanced Programming Course Assignment 1
 *  Shell Command Header File
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

#ifndef _SHELL_COMMAND_H
#define _SHELL_COMMAND_H

/********************/
/* Includes Section */
/********************/
#include "shell_def.h"
#include <stdbool.h>

/*******************/
/* Structs Section */
/*******************/

/*
 * @brief The command struct.
 * @param command The command.
 * @param status The status of the command (0 if succeeded, 1 if failed).
 * @param isInternal True if the command is an internal command, False otherwise.
 * @param background True if the command is a background command, False otherwise.
 */
typedef struct Command {
    char *command;
    int status;
    bool isInternal;
    bool background;
} Command, *PCommand;

/*********************/
/* Functions Section */
/*********************/

/*
 * @brief Create a command.
 * @param command The command.
 * @param isInternal True if the command is an internal command, False otherwise.
 * @param background True if the command is a background command, False otherwise.
 * @return The command.
 */
PCommand create_command(char *command, bool isInternal, bool background);

/*
 * @brief Destroy a command.
 * @param command The command.
 * @return void (nothing).
 */
void destroy_command(PCommand command);

#endif // _SHELL_COMMAND_H