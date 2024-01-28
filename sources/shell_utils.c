/*
 *  Advanced Programming Course Assignment 1
 *  Shell Utils Implementation File
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

#include "../include/shell_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char **tokenize_command(const char *command, int num_tokens) {
    if (command == NULL)
    {
        fprintf(stderr, "Error: tokenize_command() failed: command is NULL\n");
        return NULL;
    }

    else if (num_tokens < 1)
    {
        fprintf(stderr, "Error: tokenize_command() failed: num_tokens is less than 1\n");
        return NULL;
    }

    char **tokens = (char **)calloc(num_tokens + 1, sizeof(char *));
    bool in_quotes = false;
    int token_length = 0;
	int word = 0;

	if (tokens == NULL)
	{
		perror("Internal error: System call faliure: calloc(3)");
		return NULL;
	}

    char *token = (char *)calloc(strlen(command) + 1, sizeof(char));

    if (token == NULL)
    {
        perror("Internal error: System call faliure: calloc(3)");
        free(tokens);
        return NULL;
    }

    for (int i = 0; *(command + i) != '\0'; i++)
	{
        if (*(command + i) == '"')
            in_quotes = !in_quotes;
        
		else if (*(command + i) == ' ' && !in_quotes)
		{
            *(token + token_length) = '\0';
            *(tokens + word) = token;
			
            token = (char *)calloc(strlen(command) + 1, sizeof(char));

			if (token == NULL)
			{
				perror("Internal error: System call faliure: calloc(3)");

                for (int j = 0; j < word; j++)
                    free(*(tokens + j));
                
				return NULL;
			}

			word++;

            token_length = 0;
        }
        
		else
		{
            *(token + token_length) = *(command + i);
            token_length++;
        }
    }

    *(token + token_length) = '\0';
    *(tokens + word) = token;

    if (strcmp(*(tokens + word), "&") == 0)
    {
        free(*(tokens + word));
        *(tokens + word) = NULL;
    }

	// Set the last argument to NULL, as required by execvp.
	*(tokens + num_tokens) = NULL;

    return tokens;
}

void parse_variables(char **command, PLinkedList variableList) {
    if (command == NULL)
    {
        fprintf(stderr, "Error: parse_variables() failed: command is NULL\n");
        return;
    }

    else if (variableList == NULL)
    {
        fprintf(stderr, "Error: parse_variables() failed: variableList is NULL\n");
        return;
    }

    // If no variables are defined, return, we have nothing to do. :(
    if (variableList->head == NULL)
        return;

    for (int i = 0; *(command + i) != NULL; i++)
    {
        if (**(command + i) == '$')
        {
            char *variable_name = *(command + i) + 1;
            PNode variable = (PNode)variableList->head;
            PVariable variable_data = NULL;

            while (variable != NULL)
            {
                variable_data = (PVariable)variable->data;

                if (strcmp(variable_data->name, variable_name) == 0)
                    break;

                variable = variable->next;
            }

            // Variable not found, don't parse it.
            if (variable == NULL)
                return;

            char *tmp = (char *)calloc(strlen(variable_data->value) + 1, sizeof(char));

            if (tmp == NULL)
            {
                perror("Internal error: System call faliure: calloc(3)");
                return;
            }

            free(*(command + i));
            strcpy(tmp, variable_data->value);

            *(command + i) = tmp;
        }
    }
}