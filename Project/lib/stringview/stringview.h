/**
 * @file stringview.h
 * @author UniqueIdent
 * @brief A short and simple implementation of a stringview struct.
 * @version 0.1
 * @date 2026-06-02
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once

typedef struct{
    const char* start;
    const char* end;
}stringview;



/**
 * @brief Gets the length of the stringview, returns -1 if the stringview is considered invalid.
 * 
 * @param strview 
 * @return int Returns length of stringview, -1 if invalid.
 */
int view_length(stringview* strview);

/**
 * @brief Returns the size of the stringview, which is length+1.
 * 
 * @param strview 
 * @return int Returns size of stringview, -1 if invalid.
 */
int view_size(stringview* strview);

/**
 * @brief Copies the given stringview into a new buffer.
 * 
 * @param strview 
 * @return char* 
 */
void copy_substring(stringview* strview, char* dest);

/**
 * @brief Copies the given stringview (or after n characters stops)
 * 
 * @param strview 
 * @param n 
 * @return char* 
 */

void copy_nsubstring(stringview* strview, int n, char* dest);


/**
 * @brief Creates a stringview from a start and end pointer.
 * 
 * @param start 
 * @param end 
 * @return stringview (stringview values will be NULL if invalid input)
 */
stringview to_stringview(char* start, char* end);

/**
 * @brief Creates a stringview from a start pointer and a length.
 * 
 * @param start 
 * @param n 
 * @return stringview (stringview values will be NULL if invalid input)
 */
stringview to_nstringview(char* start, int n);