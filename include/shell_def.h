/*
 *  Advanced Programming Course Assignment 1
 *  Shell Definitions Header File
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

#ifndef _SHELL_DEF_H
#define _SHELL_DEF_H

/*
 * Allow the GNU extensions of POSIX functions (such as getline) to be used.
 * This is required for the shell to work properly.
 */
#if !defined(_XOPEN_SOURCE) && !defined(_POSIX_C_SOURCE)
	#if __STDC_VERSION__ >= 199901L
		#define _XOPEN_SOURCE 600 /* SUS v3, POSIX 1003.1 2004 (POSIX 2001 + Corrigenda) */
	#else
		#define _XOPEN_SOURCE 500 /* SUS v2, POSIX 1003.1 1997 */
	#endif					  /* __STDC_VERSION__ */
#endif					  /* !_XOPEN_SOURCE && !_POSIX_C_SOURCE */

/********************/
/* Includes Section */
/********************/

#include <bits/stdint-uintn.h>

/*******************/
/* Command Aliases */
/*******************/

/*
 * @brief Alias for the exit command.
 * @note Used to indicate that the user wants to exit the shell.
 * @note This an internal command required by the assignment.
 */
#define SHELL_CMD_EXIT "quit"

/*
 * @brief Alias for the cd command.
 *
 * @note Used to indicate that the user wants to change the current working directory.
 * @note This an internal command required by the assignment.
 */
#define SHELL_CMD_CD "cd"

/*
 * @brief Alias for the pwd command.
 * @note Used to indicate that the user wants to print the current working directory.
 * @note This an internal command required by the assignment.
 */
#define SHELL_CMD_PWD "pwd"

/*
 * @brief Alias for the clear command.
 * @note Used to indicate that the user wants to clear the screen.
 * @note This is a custom made command and is not part of the assignment.
 */
#define SHELL_CMD_CLEAR "clear"

/*
 * @brief Alias for the history command.
 * @note Used to indicate that the user wants to print the command history.
 * @note This is a custom made command and is not part of the assignment.
 */
#define SHELL_CMD_HISTORY "history"

/*
 * @brief Print the license of the shell.
 * @note This is a custom made command and is not part of the assignment.
 */
#define SHELL_CMD_GNU "gnu"

/**********************/
/* Clean screen stuff */
/**********************/

/*
 * @brief Clear and flush the screen.
 * @note Used to clear the screen and flush the output buffer.
 */
#define SHELL_CMD_CLEAR_FLUSH "\33[H\33[2J"

/*
 * @brief Clear and flush the screen, command length.
 * @note Indicates the length of the clear and flush command.
 */
#define SHELL_CMD_CLEAR_FLUSH_LEN 7

/******************/
/* Error Messages */
/******************/

/*
 * @brief Can't redirect input twice error message.
 * @note Used to indicate that the user tried to redirect input twice.
 */
#define SHELL_ERR_REDIRECT_IN_TWICE "Shell internal error: Can't redirect input twice"

/*
 * @brief Can't redirect output twice error message.
 * @note Used to indicate that the user tried to redirect output twice.
 */
#define SHELL_ERR_REDIRECT_OUT_TWICE "Shell internal error: Can't redirect output twice"

/*
 * @brief Redirecting output to a file in the first pipe is not allowed.
 * @note Used to indicate that the user tried to redirect output to a file in the first pipe.
 */
#define SHELL_ERR_REDIRECT_OUT_IN_FIRST_PIPE "Shell internal error: Redirecting output to a file in the first pipe is not allowed"

/*
 * @brief Redirecting between pipes is not allowed.
 * @note Used to indicate that the user tried to redirect between pipes.
 */
#define SHELL_ERR_REDIRECT_BETWEEN_PIPES "Shell internal error: Redirecting between pipes is not allowed"

/*
 * @brief Redirecting input from a file in the last pipe is not allowed.
 * @note Used to indicate that the user tried to redirect input from a file in the last pipe.
 */
#define SHELL_ERR_REDIRECT_IN_IN_LAST_PIPE "Shell internal error: Redirecting input from a file in the last pipe is not allowed"

/*
 * @brief Change directory error message: no such file or directory.
 * @note Used to indicate a change directory failure, and print the error.
 */
#define SHELL_ERR_CMD_CD "cd: No such file or directory"

/*
 * @brief Change directory error message: too many arguments.
 * @note Used to indicate that the used provided too many arguments to the cd command.
 */
#define SHELL_ERR_CMD_CD_ARG "cd: Too many arguments provided"

/*******************/
/* Program details */
/*******************/

/*
 * @brief Program license text.
 */
#define SHELL_CMD_GNU_LICENSE "This program is free software: you can redistribute it and/or modify\n" \
							  "it under the terms of the GNU General Public License as published by\n" \
							  "the Free Software Foundation, either version 3 of the License, or\n"    \
							  "(at your option) any later version.\n"                                  \
							  "This program is distributed in the hope that it will be useful,\n"      \
							  "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"       \
							  "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"        \
							  "GNU General Public License for more details.\n"                         \
							  "You should have received a copy of the GNU General Public License\n"    \
							  "along with this program.  If not, see <https://www.gnu.org/licenses/>.\n"

/****************/
/* Enumerations */
/****************/

/*
 * @brief Command type enum.
 * @note Used to indicate whether a command is internal, external or a control command.
 */
typedef enum _CommandType
{
	Internal = 0,
	External = 1,
	ControlCommand = 2
} CommandType;

/*
 * @brief Result enum.
 * @note Used to indicate success or failure of an internal command.
 */
typedef enum _Result
{
	Success = 0,
	Failure = 1
} Result;

/***********************************/
/* Internal Settings for the shell */
/***********************************/

/*
 * @brief Maximum command length.
 * @note Commands longer than this will be truncated.
 * @note The default value is 1024 characters.
 */
#define SHELL_MAX_COMMAND_LENGTH 1024

/*
 * @brief Maximum path length.
 * @note Paths longer than this will be truncated.
 * @note The default value is 512 characters.
 */
#define SHELL_MAX_PATH_LENGTH 512

/*
 * @brief The default prompt for the shell.
 */
#define SHELL_DEFAULT_PROMPT "hello"

#endif /* _SHELL_DEF_H */