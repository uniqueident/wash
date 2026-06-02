#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtin.h"
#include "parseargs.h"
#include "stringview.h"
#include "vector.h"

//I really wanted the Ansi codes. cheatsheet here: https://gist.github.com/ConnerWill/d4b6c776b509add763e17f9f113fd25b
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_RESET   "\x1b[0m"

#define MAX_SIZE (256 + 1)
int input(char* buf, int size){

    if(fgets(buf, size, stdin) == NULL){
        printf("Failure to read input, trying again.\n");
        return -1;
    };
    //Safety check. Truncate input as needed.
    //Prevent keeping the trailing newline character, which is added by fgets
    int len = strlen(buf);
    if(len > 0){
        buf[len-1]  = '\0'; //remove newline character
    }

    // Drain stdin. No fflush. Is UB.
    if(len >= MAX_SIZE-1){
        int c;
        while((c = fgetc(stdin)) != '\n' && c != EOF);
    }

    return 0;
}


int main(int argc, char** argv){
    //Allow command line arguments as an initial thing.
    if(argc>2){
        printf("Use <-h> for help!\n");
        return 0;
    }else if (argc == 2) {
        if(strcmp(argv[1],"-h") == 0){
            execute(builtin_command_ID("help"),NULL);
            return 0;
        }
    }

    static char buf[MAX_SIZE] = {0};
    vector_t vec = vec_create();
    create_paths_vector();
    while(true){
        char* cwd = getcwd(buf,sizeof(buf));
        if(cwd == NULL){
            printf("buffer too large");
        }else{
            printf("\x1b[1;35m %s:\x1b[0m",cwd);
        }

        memset(buf, 0, sizeof(buf));
        if(input(buf,sizeof(buf)) == -1){
            continue;
        }

        vec = strparse(buf,&vec);
        if(vec == NULL || vec_size(&vec)<=0){
            printf("No arguments detected.\n");
            continue;
        }

        char* cmd = get(&vec,0);
        if(strcmp(cmd, "exit") == 0){
            destroy(&vec);
            destroy_paths_vector();
            return 0;
        }else{
            execute(builtin_command_ID(cmd), &vec);
        }

        clean(&vec);
   
    }
    return 0;
}