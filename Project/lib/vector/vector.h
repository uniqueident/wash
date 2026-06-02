/**
 * @file vector.h
 * @author uniqueident
 * @brief A simple and custom implementation of a vector in C.
 * @version 0.1
 * @date 2026-06-02
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

//Opaque struct. This is intentional.
typedef struct vector* vector_t;

/**
 * @brief Get the raw array object
 * 
 * @param vec 
 * @return char** 
 */
char** get_raw_array(vector_t* vec);
/**
 * @brief Pushes the vector back. This will always take O(n) time. Use sparingly.
 * 
 * @param vec 
 * @param str String to place on the front.(This should be allocated before being passed in, and then managed by the vector)
 * @return int 
 */
int push_back(vector_t* vec,char* str);

/**
 * @brief Adds to the end of the vector, appends at the end, this results in the least amount of movement.
 * 
 * @param vec 
 * @param str String to place in the back. (This should be allocated before being passed in, and then managed by the vector)
 * @return int 
 */
int append(vector_t* vec, char* str);

/**
 * @brief Works similarly to how you would get the value at the index of an array.
 * 
 * @param vec 
 * @param index Integer value that is non-zero and in the range of 0 to size-1.
 * @return char* 
 */
char* get(vector_t* vec ,int index);

/**
 * @brief Returns the size of the vector (Tip: max index = size-1)
 * 
 * @param vec 
 * @return int 
 */
int vec_size(vector_t* vec);


/**
 * @brief Similar in function to a destructor in C++
 * 
 * @param vec 
 */
void destroy(vector_t*  vec);

/**
 * @brief Clears all of the vector entries, does not free the vector itself. Enables reuse.
 * 
 * @param vec 
 */
void clean(vector_t* vec);

/**
 * @brief "Deepcopy" of the underlying vector, returns it as an array of strings, with a null terminaton.
 * 
 * @param vec 
 * @return char** (Array of nullbyte character strings.)
 */
char** copy_to_array(vector_t* vec);



vector_t vec_create();
