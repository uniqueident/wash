#pragma once
//test
#include "vector.h"

typedef struct{
    int count;
    char** arguments;
} args_t;

vector_t strparse(char* input, vector_t* vec);