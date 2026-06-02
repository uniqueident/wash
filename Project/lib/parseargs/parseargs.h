/**
 * @file parseargs.h
 * @author UniqueIdent
 * @brief A header file for simple string parsing using space and tab as delimiters.
 * @version 0.1
 * @date 2026-06-02
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once
//test
#include "vector.h"

/**
 * @brief Takes in a string, and parses it into a vector of strings, using space and tab as delimiters.
 * 
 * @param input The string to parse
 * @param vec The vector to write the parsed strings to.
 * @return vector_t Returns the modified vector, or NULL if there is a failure. Vector is modified in place.
 */
vector_t strparse(char* input, vector_t* vec);