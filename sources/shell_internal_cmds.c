/*
 *  Advanced Programming Course Assignment 1
 *  Shell internal commands Implementation File
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

#include "../include/shell_internal_cmds.h"
#include "../include/shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

Result cmdCD(char *path, int argc) {
    // Only one argument is allowed, like in the original shell.
    if (argc > 2) {
        fprintf(stderr, "%s\n", SHELL_ERR_CMD_CD_ARG);
        return Failure;
    } else if (argc == 2) {
        // Home directory.
        if (strcmp(path, "~") == 0) {
            // Save current working directory before changing it.
            getcwd(workingdir, SHELL_MAX_PATH_LENGTH);

            if (chdir(homedir) == -1) {
                perror("Internal error: System call faliure: chdir(2)");
                return Failure;
            }

            return Success;
        }

            // Previous directory.
        else if (strcmp(path, "-") == 0) {
            if (strcmp(workingdir, "") == 0)
                return Success;

            if (chdir(workingdir) == -1) {
                perror("Internal error: System call faliure: chdir(2)");
                return Failure;
            }

            getcwd(workingdir, SHELL_MAX_PATH_LENGTH);

            return Success;
        }

        getcwd(workingdir, SHELL_MAX_PATH_LENGTH);

        if (chdir(path) == -1) {
            fprintf(stderr, "%s\n", SHELL_ERR_CMD_CD);
            return Failure;
        }
    }

        // No arguments - go to home directory.
    else {
        getcwd(workingdir, SHELL_MAX_PATH_LENGTH);

        if (chdir(homedir) == -1) {
            perror("Internal error: System call faliure: chdir(2)");
            return Failure;
        }
    }

    return Success;
}

Result cmdPWD() {
    fprintf(stdout, "%s\n", cwd);
    return Success;
}

Result cmdClear() {
    // Write the clear screen command to the standard output.
    write(STDOUT_FILENO, SHELL_CMD_CLEAR_FLUSH, SHELL_CMD_CLEAR_FLUSH_LEN);
    return Success;
}

Result cmdChangePrompt(char *new_prompt) {
    if (new_prompt == NULL) {
        fprintf(stderr, "%s\n", SHELL_ERR_CMD_CHANGE_PROMPT_SYNTAX);
        return Failure;
    }

    if (strlen(new_prompt) > SHELL_MAX_PATH_LENGTH) {
        fprintf(stderr, "%s\n", SHELL_ERR_CMD_CHANGE_PROMPT_LONG);
        return Failure;
    }

    strcpy(curr_prompt, new_prompt);
    return Success;
}

Result cmdrepeatLastCommand() {
    if (commandHistory->tail == NULL) {
        fprintf(stderr, "No commands in history.\n");
        return Failure;
    }

    PCommand lastCommand = (PCommand)commandHistory->tail->data;
    if (lastCommand == NULL || strlen(lastCommand->command) == 0) {
        fprintf(stderr, "No last command to repeat.\n");
        return Failure;
    }

    // Resend the last command to the parser and executor
    char **argv = NULL;

    if (parse_command(lastCommand->command, &argv) == External)
        execute_command(argv);

    // Free argv after execution
    if (argv != NULL)
    {
        for (size_t k = 0; *(argv + k) != NULL; ++k)
            free(*(argv + k));
        
        free(argv);
    }

    return Success;
}


Result setVariable(char *name, char *value) {
    // Check if the variable already exists. If yes, update its value.
    PNode curr = variableList->head;
    while (curr != NULL)
    {
        PVariable variable = (PVariable) (curr->data);
        if (strcmp(variable->name, name) == 0)
        {
            char *tmp = (char *) calloc(strlen(value) + 1, sizeof(char));

            if (tmp == NULL) {
                perror("Error: setVariable() failed: malloc() failed");
                return Failure;
            }

            strcpy(tmp, value);
            free(variable->value);
            variable->value = tmp;
            return Success;
        }
        curr = curr->next;
    }

    // Add a new variable
    PVariable variable = create_variable(name, value);
    if (variable == NULL)
    {
        fprintf(stderr, "Error: setVariable() failed: create_variable() failed\n");
        return Failure;
    }

    if (addNode(variableList, variable) == 1)
    {
        fprintf(stderr, "Error: setVariable() failed: add_node() failed\n");
        destroy_variable(variable);
        return Failure;
    }

    return Success;
}

Result cmdRead(char *variableName) {
    char input[SHELL_MAX_COMMAND_LENGTH] = {0};

    if (fgets(input, sizeof(input), stdin) == NULL)
        return Failure;  // Error or end-of-file

    // Remove newline character
    *(input + strcspn(input, "\n")) = '\0';

    return setVariable(variableName, input);
}

Result cmdHistory(int argc) {
    if (argc > 2) {
        fprintf(stderr, "Too many arguments.\n");
        return Failure;
    }

    // Print all history
    PNode curr = commandHistory->head;
    int i = 1;
    fprintf(stdout, "Command History:\n");
    fprintf(stdout, "#\tCMD\tSTAT\tINT\tBACK\n");

    while (curr != NULL)
    {
        PCommand command = (PCommand) (curr->data);
        fprintf(stdout, "%d\t%s\t%s\t%s\t%s\n", i, command->command, 
            (command->status == 0 ? "SUCC":"FAIL"), 
            (command->isInternal == 0 ? "NO":"YES"), 
            (command->background == 0 ? "NO":"YES"));
        curr = curr->next;
        i++;
    }

    return Success;
}

Result cmdIfElse(char *condition, char *thenCmd, char *elseCmd) {
    int status = system(condition); // Execute the condition command

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        // Condition succeeded, execute thenCmd
        system(thenCmd);
    } else {
        // Condition failed, execute elseCmd
        if (elseCmd != NULL) {
            system(elseCmd);
        }
    }

    return Success;
}
