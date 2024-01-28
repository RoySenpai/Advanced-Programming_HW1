/*
 *  Advanced Programming Course Assignment 1
 *  Shell Variable Implementation File
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

#include "../include/Variables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PVariable create_variable(char *name, char *value) {
    if (name == NULL)
    {
        fprintf(stderr, "Error: create_variable() failed: name is NULL\n");
        return NULL;
    }

    else if (value == NULL)
    {
        fprintf(stderr, "Error: create_variable() failed: value is NULL\n");
        return NULL;
    }

    PVariable variable = (PVariable)malloc(sizeof(Variable));

    if (variable == NULL)
    {
        perror("Error: create_variable() failed: malloc() failed");
        return NULL;
    }

    variable->name = (char *)malloc(strlen(name) + 1);

    if (variable->name == NULL)
    {
        perror("Error: create_variable() failed: malloc() failed");
        free(variable);
        return NULL;
    }

    strcpy(variable->name, name);

    variable->value = (char *)malloc(strlen(value) + 1);

    if (variable->value == NULL)
    {
        perror("Error: create_variable() failed: malloc() failed");
        free(variable->name);
        free(variable);
        return NULL;
    }

    strcpy(variable->value, value);

    return variable;
}

void destroy_variable(PVariable variable) {
    if (variable == NULL)
    {
        fprintf(stderr, "Error: destroy_variable() failed: variable is NULL\n");
        return;
    }

    free(variable->name);
    free(variable->value);
    free(variable);
}