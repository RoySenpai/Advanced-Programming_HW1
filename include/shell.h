/*
 *  Advanced Programming Course Assignment 1
 *  Shell Header File
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

#ifndef _SHELL_H
#define _SHELL_H

/********************/
/* Includes Section */
/********************/
#include "shell_internal_cmds.h"


/*********************/
/* Functions Section */
/*********************/

/*
 * @brief Parse a command into arguments.
 * @param command The command to parse.
 * @param argv The array of arguments.
 * @return 0 if it an internal command, 1 if it is an external command.
 * @note This function will modify the argv array.
 * @note This function will execute internal commands.
 */
CommandType parse_command(char *command, char **argv);

/*
 * @brief Execute change directory command.
 * @param path The path to change to.
 * @param argc The number of arguments.
 * @return Success if the command succeeded, Failure otherwise.
 * @note number of arguments must be 1.
 */
Result cmdCD(char *path, int argc);

/*
 * @brief Execute a command.
 * @param argv The array of arguments.
 * @return void (nothing).
 */
void execute_command(char **argv);

/*
 * @brief A signal handler for the shell program.
 * @param signum The signal number.
 * @noreturn
 */
void shell_sig_handler(int signum);

#endif /* _SHELL_H */