#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>
#include <unistd.h>
#include "parseargs.h"
#include "vector.h"


#define MAX_SIZE (256 + 1)
/**
 * @brief This is identical to wash.c input, but a small change was needed so it was redefined for speed 
 of development sake. (It is not good practice).
 * 
 * @param buf 
 * @param size 
 * @return int 
 */
int input(char* buf, int size){

    if(fgets(buf, size, stdin) == NULL){
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


/**
 * @brief Self explanatory, prints the helper text.
 * 
 */
void help(){
    printf("This application reads the first [n] lines specified of a file [filepath]\n");
}

static char buf[MAX_SIZE] = {0};

/**
 * @brief This is head_nine, a simple application that reads the first n lines of a file.
 If given no file, it reads from stdin.
 If given no n, it defaults to 9.
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char** argv){
    if(argc>4){
        printf("Too many args 2 max\n");
        return -1;
    }

    if(argc > 1 && strcmp(argv[1],"-h") == 0){
        help();
        return 0;
    }
    //Imagine if I could auto generate these structs...
    typedef struct{
        bool n_flag;
        uint32_t n;
        bool file_flag;
        char* fp;
    }flags;

    flags input_flags = {
        .n_flag =false,
        .n = 9,
        .file_flag = false,
        .fp = NULL
    };

    //Parse args, hacky.
    for(int i = 1; i<argc; i++){
        if(strcmp(argv[i], "-h") == 0){
            printf("Bad input. Shutting down.\n");
            return -1;
        }

        if(strcmp(argv[i],"-n") ==0){
            input_flags.n_flag = true;
        }else if(input_flags.n_flag){
            if((input_flags.n = atoi(argv[i])) == 0 || input_flags.n <0){
                printf("Invalid number provided for n\n");
                return -1;
            }else{
                input_flags.n_flag = false; //reset flag, in case of multiple -n flags.
            }
        }else{
            input_flags.file_flag = true;
            input_flags.fp = argv[i];
        }
    }

    FILE* file;
    int i = 0;
    //No file provided, read from stdin.
    if(input_flags.file_flag == false){
        bool read = true;
        while(i<input_flags.n){
            if(input(buf,sizeof(buf)) == -1){
                return 0;
            }else{
                printf("%s\n", buf);
                memset(buf, 0, sizeof(buf));
                i++;
            }
        }
        return 0;

    }else{
        file = fopen(input_flags.fp, "r");
    }

    //open the file.
    if(file == NULL){
        printf("Unable to open file |%s|\n", input_flags.fp);
        return -1;
    }
    //file provided, read from file.
    while(i<input_flags.n){
        if(fgets(buf, sizeof(buf), file) == NULL){
            fclose(file);
            return 0;
        }else{
            printf("%s", buf);
            memset(buf, 0, sizeof(buf));
        }
        i++;
    }
    //cleanup.
    fclose(file);
    return 0;
}