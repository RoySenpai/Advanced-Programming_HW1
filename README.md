# Advanced Programming Course Assignment 1
### For Computer Science B.S.c Ariel University
**By Roy Simanovich and Almog Shor**

## Description
In this assignment we were asked to create a shell program that will be able to run commands and programs.

The shell supports the following internal commands:
* **`cd`** - change directory
* **`pwd`** - print working directory.
* **`history`** - print all the commands that were entered to the shell.
* **`!!`** - run the last command that was entered (if exists).
* **`clear`** - clear the screen.
* **`quit`** - exit the shell.
* **`read`** - read a string from the user and save it to a variable. (e.g. `read var`).
* **`prompt`** - change the shell prompt. (e.g. `prompt = $`).

The shell also supports redirection of the standard input, output and error streams using the following operators:
* **`>`** - redirect the standard output to a file. (e.g. `ls > file.txt`).
* **`>>`** - redirect the standard output to a file and append it to the end of the file. (e.g. `ls >> file.txt`).
* **`<`** - redirect the standard input from a file. (e.g. `sort < file.txt`).
* **`2>`** - redirect the standard error to a file. (e.g. `ls 2> file.txt`).

The shell also supports piping between commands using the **`|`** operator. (e.g. `ls | sort`). Please note that the shell supports also multiple pipes (e.g. `ls | sort | uniq`) and redirections (e.g. `ls | sort > file.txt`).

The shell also supports control operators:
* **`&`** - run the command in the background. (e.g. `sleep 10 &`).
* **`if`** - create an if statement.
* **`then`** - run the command after the **`then`** only if the command succeeded.
* **`else`** - run the command after the **`else`** only if the command failed.
* **`fi`** - end the if statement.

The shell supports the following expansions:
* **`$var`** - expand the variable **`var`**.
* **`$?`** - expand the exit status of the last command.

You can use **``$var = value``** to set a variable with a value.

## Requirements
* Linux machine (Ubuntu 22.04 LTS preferable)
* GNU C Compiler
* Make

## Building
```
# Cloning the repo to local machine.
git clone https://github.com/RoySenpai/Advanced-Programming_HW1.git

# Building all the necessary files & the main program.
make all
```

## Running
```
# Run the shell.
./myshell
```
