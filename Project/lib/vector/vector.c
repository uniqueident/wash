/*
Course number: CSC 3350
Assignment Title: Wash Shell
Professor: Andy Cameron
Date: 06/02/2026
*/
#include "vector.h"
#include <stdlib.h>
#include <string.h>
//The actual structure.
struct vector{
    int size;
    int capacity;
    char** array;
};

void clean(vector_t* vec){
    if(vec == NULL){
        return;
    }
    for(int i = 0; i<(*vec)->size; i++){
        if((*vec)->array[i] != NULL){
            free((*vec)->array[i]);
        }
    }
    (*vec)->size = 0;
}


char** get_raw_array(vector_t* vec){
    if((*vec) == NULL){
        return NULL;
    }
    return (*vec)->array;
}

/**
 * @brief Does the actual resizing, does not return, but does not modify the given vector if it fails.
 * 
 * @param vec 
 */
void resize(vector_t* vec){
    //double capacity.
    int cap = (*vec)->capacity*2;
    //reallocate array (supposedly according to C99 this frees the old memory(if it succeeds))
    char** new_arr = realloc((*vec)->array,sizeof(char*)*cap);
    //check if success
    if(new_arr == NULL){
        return;
    }
    //update capacity
    (*vec)->capacity = cap;
    (*vec)->array= new_arr;
}
/**
 * @brief Tries to resize the vector. returns -1 on failure.
 * 
 * @param vec 
 * @return int 
 */
int try_resize(vector_t* vec){
    if(vec == NULL){
        return -1;
    }
    if((*vec)->size == (*vec)->capacity){
        resize(vec);
        if((*vec)->size == (*vec)->capacity){
            return -1;
        }
    }
    return 0;
}

int push_back(vector_t* vec,char* str){
    if(vec == NULL || str == NULL){
        return -1;
    }

    int ret = try_resize(vec);
    if(ret !=0){
        return ret;
    }

    for(int i = (*vec)->size-1; i>0; i--){
        (*vec)->array[i] = (*vec)->array[i-1];
    }
    (*vec)->array[0] = str;
    (*vec)->size += 1;
    return 0;
}

int append(vector_t* vec, char* str){
    if(vec == NULL || str == NULL){
        return -1;
    }
    
    int ret = try_resize(vec);
    if(ret !=0){
        return ret;
    }

    (*vec)->array[(*vec)->size] = str;
    (*vec)->size += 1;
    return 0;
}

char* get(vector_t* vec ,int index){
    if(index >= (*vec)->size || index<0){
        return NULL;
    }
    return (*vec)->array[index];
}

int vec_size(vector_t* vec){
    return (*vec)->size;
}

void destroy(vector_t* vec){
    if(vec == NULL || *vec == NULL){
        return;
    }
    char** arr = (*vec)->array;
    for(int i=0; i<(*vec)->size; i++){
        if(arr[i] != NULL){
            free(arr[i]);
            arr[i] = NULL;
        }
        
    }
    free((*vec)->array);
    free(*vec);
}

char** copy_to_array(vector_t* vec){
    if((*vec) == NULL){
        return NULL;
    }
    //allocate full array
    char** arr = calloc((*vec)->size+1, sizeof(char*));
    //capture each string
    for(int i = 0; i<(*vec)->size; i++){
        //make space for each string
        arr[i] = malloc(strlen((*vec)->array[i])+1);
        //copy each string
        strcpy(arr[i],(*vec)->array[i]);
    }
    //set null ending because some programs use this.
    arr[(*vec)->size] = NULL;
    return arr;
}

vector_t vec_create(){
    vector_t vec = malloc(sizeof(struct vector));
    if(vec == NULL){
        return NULL;
    }
    vec->size =0;
    vec->capacity = 4;
    vec->array = calloc(vec->capacity,sizeof(char*));
    if(vec->array == NULL){
        free(vec);
        return NULL;
    }
    return vec;
}
