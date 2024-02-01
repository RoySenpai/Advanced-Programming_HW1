/*
 *  Advanced Programming Course Assignment 1
 *  Shell internal commands Header File
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

#ifndef _SHELL_CD_H
#define _SHELL_CD_H

/********************/
/* Includes Section */
/********************/
#include "shell_def.h"
#include "Command.h"
#include "Variables.h"
#include "LinkedList.h"
#include <stdbool.h>

/********************/
/* External Section */
/********************/
extern char *homedir;
extern char *cwd;
extern char *workingdir;
extern char *curr_prompt;

extern PLinkedList commandHistory;
extern PLinkedList variableList;


/*********************/
/* Functions Section */
/*********************/

/*
 * @brief Execute change directory command.
 * @param path The path to change to.
 * @param argc The number of arguments.
 * @return Success if the command succeeded, Failure otherwise.
 * @note number of arguments must be 2.
 */
Result cmdCD(char *path, int argc);

/*
 * @brief Execute print working directory command.
 * @return Success always.
 */
Result cmdPWD();

/*
 * @brief Execute clear command.
 * @return Success always.
 */
Result cmdClear();

/*
 * @brief Execute change prompt command.
 * @param new_prompt The new prompt.
 * @return Success if the command succeeded, Failure otherwise.
 */
Result cmdChangePrompt(char *new_prompt);

/*
 * @brief Execute last command.
 * @return Success if the command succeeded, Failure otherwise.
 */
Result cmdrepeatLastCommand();


/*
 * @brief Execute set variable command.
 * @param name The name of the variable.
 * @param value The value of the variable.
 * @return Success if the command succeeded, Failure otherwise.
 */
Result setVariable(char *name, char *value);

/*
 * @brief Execute read variable command.
 * @param variableName The name of the variable.
 * @return Success if the command succeeded, Failure otherwise.
 */
Result cmdRead(char *variableName);

/*
 * @brief Execute history command.
 * @param argc The number of arguments.
 * @return Success if the command succeeded, Failure otherwise.
 */
Result cmdHistory(int argc);

#endif /* _SHELL_CD_H */