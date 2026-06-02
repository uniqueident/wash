/**
 * @file builtin.h
 * @author UniqueIdent
 * @brief A header file to run the builtin shell commands. These commands should be run by the shell itself. And failing that, attempting to run executables in the PATH vector.
 * @version 0.1
 * @date 2026-06-02
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once
#include "vector.h"
#define DELIM " "

/*
These two functions are in charge of the lifetime of the PATH vector for the shell.
Only commands found in the path (outside of builtin commands) will be executed.
It will not check the current directory.
*/

/**
 * @brief Create a paths vector object, and adds the default path of /bin to it
 If the vector exists, this function does nothing.
 * 
 */
void create_paths_vector();
/**
 * @brief Destroys the paths vector object, and frees memory associated with it.
 * 
 */
void destroy_paths_vector();

/**
 * @brief Returns the integer ID of the builtin command. -1 if not found.
 * 
 * @param command A string.
 * @return const int 
 */
const int builtin_command_ID(const char* command);

/**
 * @brief Makes the assumption that the input is space delimited and formatted at this point.
 * 
 * @param cmd_id The integer ID of the command to execute.
 * @param arg arguments to the command. As with the standard methods, the first argument of the command is the command name.
 * @return int will return 0 on success, -1 on failure. Failure just means that the command could not be executed.
 */
int execute(const int cmd_id, vector_t* arg);


