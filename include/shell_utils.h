/*
 *  Advanced Programming Course Assignment 1
 *  Shell Utils Header File
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

#ifndef _SHELL_UTILS_H
#define _SHELL_UTILS_H

/********************/
/* Includes Section */
/********************/
#include "shell_def.h"
#include "LinkedList.h"
#include "Variables.h"


/*********************/
/* Functions Section */
/*********************/

/*
 * @brief Tokenize a command into tokens.
 * @param command The command to tokenize.
 * @param num_tokens The number of tokens.
 * @return The array of tokens.
 * @note This function won't modify the command.
 */
char **tokenize_command(const char *command, int num_tokens);

/*
 * @brief Parse command variables and replace them with their values.
 * @param command The command to parse.
 * @param variableList The list of variables.
 */
void parse_variables(char **command, PLinkedList variableList);


#endif // _SHELL_UTILS_H