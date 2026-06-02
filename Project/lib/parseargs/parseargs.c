#include "stringview.h"
#include "parseargs.h"
#include <stdlib.h>


vector_t strparse(char* input, vector_t* vec){
    if(input == NULL || vec == NULL){
        return NULL;
    }
    stringview view = {.start = NULL, .end = NULL};
    char* start = NULL;

    //Iterate
    while(*input != '\0'){
        //Check if whitespace. If not, then at arg.
        if(*input != ' '&& *input != '\t'){
            start = input;
            //Iterate through arg until whitespace or null.
            while(*input != ' ' && *input != '\t' && *input != '\0'){input++;}
            //view the arg!
            view = to_stringview(start, input);
            //manual allocation needed
            start = calloc(view_size(&view),sizeof(char));
            copy_substring(&view, start);
            append(vec,start);
        }
        //advance
        input++;
    }
    return *vec;
}