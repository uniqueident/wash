#pragma once
#include "vector.h"
#define DELIM " "

void create_paths_vector();
void destroy_paths_vector();

const int builtin_command_ID(const char* command);

/**
 * @brief Makes the assumption that the input is space delimited and formatted at this point.
 * 
 * @param cmd_id 
 * @param arg 
 * @return int 
 */
int execute(const int cmd_id, vector_t* arg);


