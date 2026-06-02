#pragma once

typedef struct{
    const char* start;
    const char* end;
}stringview;



int view_length(stringview* strview);

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