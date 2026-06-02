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
            start = calloc(view_size(&view),sizeof(char));
            copy_substring(&view, start);
            append(vec,start);
        }
        //advance
        input++;
    }
    return *vec;
}



/*
Okay brief description.

Make a parser that takes in a string input. The string must have the following qualities:
1. Must be null byte terminated.
2. Must be space delimited.
The string may have the following attribute:
1. Piping, and reidirection
2. Quoted strings (maybe).

After parsing, the function should return A NEWLY ALLOCATED array of strings.

This allows us to pass it into programs that take arguments.
Once passed in, note that the program CREATES A COPY of the args.
This means we can free the array after the program call.


Stringview becomes useful here. Create a stringview for EACH ARGUMENT.
This allows us to count the args, and get the length of each arg.
We can now allocate the argv array pointers. using argc.

THen once we have those views, we can use our stringview copy to copy the string.
Finally we can add 
*/
