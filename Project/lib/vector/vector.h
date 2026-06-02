#pragma once

typedef struct vector* vector_t;

char** get_raw_array(vector_t* vec);

int push_back(vector_t* vec,char* str);

int append(vector_t* vec, char* str);

char* get(vector_t* vec ,int index);

int vec_size(vector_t* vec);

void destroy(vector_t*  vec);

char** copy_to_array(vector_t* vec);

void clean(vector_t* vec);

vector_t vec_create();
