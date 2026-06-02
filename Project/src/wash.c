#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "builtin.h"
#include "parseargs.h"
#include "vector.h"

//I really wanted a colored prompt. cheatsheet here: https://gist.github.com/ConnerWill/d4b6c776b509add763e17f9f113fd25b
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_RESET   "\x1b[0m"

#define MAX_SIZE (256 + 1)
int input(char* buf, int size){

    //Select the proper stream to listen to, stdin for normal inputs, as such is hard coded. This is intentional.
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
        //needed an additional CWD call to get nice flavor text in the shell. Similar in idea to how bash shows the current directory in the prompt.
        char* cwd = getcwd(buf,sizeof(buf));
        if(cwd == NULL){
            printf("buffer too large");
        }else{
            printf("\x1b[1;35m %s:\x1b[0m",cwd);
        }
        //Since the buffer gets reused, it should be reset before each use for safety and edge cages. Its good practice.
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
        //Exit needs to be handled as a special case, otherwise we would have to exit from the execute function. 
        if(strcmp(cmd, "exit") == 0){
            destroy(&vec);
            destroy_paths_vector();
            //destruction of these vectors should occur before program exit, as this is good practice.
            return 0;
        }else{
            //The execute command determines whether or not the command is built in and whether or not it gets executed.
            execute(builtin_command_ID(cmd), &vec);
        }

        clean(&vec);
   
    }
    return 0;
}