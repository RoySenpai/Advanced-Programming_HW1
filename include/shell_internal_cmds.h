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

#include "shell_def.h"

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
 * @brief - Execute echo command.
 * @param - args The arguments array.
 * @param - argc The number of arguments.
 * @return - Success if the command succeeded, Failure otherwise.
 */
Result cmdEcho(char **args, int argc);


#endif /* _SHELL_CD_H */