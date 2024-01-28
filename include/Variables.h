/*
 *  Advanced Programming Course Assignment 1
 *  Shell Variables Header File
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

#ifndef _SHELL_VARIABLES_H
#define _SHELL_VARIABLES_H

/********************/
/* Includes Section */
/********************/
#include "shell_def.h"

/*******************/
/* Structs Section */
/*******************/

/*
 * @brief The variable struct.
 * @param name The name of the variable.
 * @param value The value of the variable.
 * @note The name and value pointers cannot be NULL.
 */
typedef struct Variable {
    char *name;
    char *value;
} Variable, *PVariable;

/*********************/
/* Functions Section */
/*********************/

/*
 * @brief Create a variable.
 * @param name The name of the variable. Name should not contain '=' and should start with a "$" sign.
 * @param value The value of the variable.
 * @return The variable.
 */
PVariable create_variable(char *name, char *value);

/*
 * @brief Destroy a variable.
 * @param variable The variable to destroy.
 */
void destroy_variable(PVariable variable);



#endif // _SHELL_VARIABLES_H