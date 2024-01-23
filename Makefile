#################################################################################
# 	Advanced Programming Course Assignment 1 Makefile		#
# 	Authors: Roy Simanovich and Almog Shor (c) 2024			#
# 	Description: This Makefile compiles the programs and libraries 		#
# 				Date: 2024-02					#
# 			Course: Advanced Programming				#
# 				Assignment: 1				#
# 				Compiler: gcc					#
# 				OS: Linux					#
# 			IDE: Visual Studio Code					#
#################################################################################

# Use the gcc compiler.
CC = gcc

# Flags for the compiler.
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -pedantic -I$(SOURCE_PATH)

# Command to remove files.
RM = rm -f

# Constants for the source, object and include paths.
SOURCE_PATH = sources
OBJECT_PATH = objects
INCLUDE_PATH = include

# Variables for the source, object and header files.
SOURCES = $(wildcard $(SOURCE_PATH)/*.c)
HEADERS = $(wildcard $(INCLUDE_PATH)/*.h)
OBJECTS = $(subst sources/,objects/,$(subst .c,.o,$(SOURCES)))

# Variables for the client and server object files.
OBJECTS_F = myshell.o shell_internal_cmds.o

# Variables for the object files.
OBJ_FILES = $(addprefix $(OBJECT_PATH)/, $(OBJECTS_F))

# Phony targets - targets that are not files but commands to be executed by make.
.PHONY: all default clean

# Default target - compile everything and create the executables and libraries.
all: myshell

# Alias for the default target.
default: all


############
# Programs #
############

# Compile the shell program.
myshell: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^


################
# Object files #
################

# Compile all the C files that are in the source directory into object files that are in the object directory.
$(OBJECT_PATH)/%.o: $(SOURCE_PATH)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


#################
# Cleanup files #
#################

# Remove all the object files, shared libraries and executables.
clean:
	$(RM) $(OBJECT_PATH)/*.o *.so myshell