#include "builtin.h"
#include "vector.h"
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAX_SIZE (256 + 1)
#define PATH_SIZE (MAX_SIZE*2 + 2)

vector_t executable_paths = NULL;

static const char* builtin_commands[] = {
    "echo",
    "pwd",
    "cd",
    "setpath",
    "help"
};

typedef enum{
 ID_echo =0 ,
 ID_pwd,
 ID_cd,
 ID_setpath,
 ID_help,
 __SENTINEL__
} Builtin;

vector_t get_path(){
    return executable_paths;
}

void create_paths_vector(){
    if(executable_paths == NULL){
        executable_paths = vec_create();
        char* initial = malloc(sizeof(char)*strlen("/bin"));
        char init[] = "/bin";
        memcpy(initial,init,sizeof(init));
        append(&executable_paths,initial);
    }
}

void destroy_paths_vector(){
    destroy(&executable_paths);
}

//as it says on the tin.
void echo(vector_t* message){
    if(message == NULL){
        return;
    }
    for(int i = 0; i <vec_size(message)-1; i++){
        printf("%s ",get(message,i));
    }
    printf("%s\n", get(message,vec_size(message)-1));
}

void pwd(){
    char cwd[MAX_SIZE+1] = {0};
    char* res = getcwd(cwd, sizeof(cwd));
    if(res == NULL){
        printf( "Unable to write current working directory to buffer\n");
    } else {
        printf("%s\n", cwd);
    }
}

int cd(vector_t* path){
    int res = 0;

    if(path == NULL){
        res = chdir(getenv("HOME"));
    }else {
        int sizing = vec_size(path);
        if(sizing<1 || sizing>2){
            printf("Command <%s> may take one argument!\n",get(path,0));
            return -1;
        }
        //always true if always two arguments.
        res = chdir(get(path,sizing-1));
    }

    if(res != 0){
        printf("Unable to change directory to %s\n", get(path,1));
    }

    return res;
}

/**
 * @brief sets the executable paths that the shell will look for for the executables.
 * 
 * @param args the paths that are space delimited.
 * @return int 
 */
struct stat* check(char* path, struct stat* sb){
    if(path == NULL){
        return NULL;
    }
    if(stat(path,sb) == -1){
        return NULL;
    }
    return sb;
}

int setpath(vector_t* args){
    if (args == NULL){
        printf("No paths provided to setpath\n");
        return -1;
    }else{
        if(vec_size(args) == 1){
            printf("Command <%s> requires at least one argument!\n",get(args,0));
            return -1;
        }

        clean(&executable_paths);
        //0 is command name
        struct stat sb;
        for(int i = 1; i<vec_size(args); i++){
            if(check(get(args,i),&sb) == NULL || !S_ISDIR(sb.st_mode)){
                printf("Not a Directory|%s|, coninuing to next argument\n",get(args,i));
                continue;
            }
            int length  = strlen(get(args,i));
            //Fix for issues with static allocated memory mixing with dynamic.
            char* addin = calloc(length+1,sizeof(char)+1);
            memcpy(addin, get(args,i), length);
            printf("Searchpath added: %s\n",addin);

            append(&executable_paths,addin);
        }
    }
    return 0;
}
/**
 * @brief Prints out the available functions and the helper text.
 * 
 */
void help(){
    printf("Available builtin commands:\n");
    for(int i=0; i<__SENTINEL__; i++){
        printf("\t%s\n", builtin_commands[i]);
    }
}

int execute_external(vector_t *argv){
    if(executable_paths != NULL && vec_size(&executable_paths)<1){
        printf("Warning, Path has been emptied. Please set a path to search for executables.\n");
    }
    if(argv == NULL || executable_paths == NULL){
        return -1;
    }
    char* cmd = get(argv,0);
    const int len = strlen(cmd);

    char buf[PATH_SIZE] = {0}; //allocated as such, since paths and files IN THEORY can be up to len*2+2 long.
    for(int i = 0; i<vec_size(&executable_paths); i++){
        char* path = get(&executable_paths,i);
        int pathlen = strlen(path);
        if(pathlen+len >= PATH_SIZE){
            printf("Path is too long");
            continue;
        }
        //Memory safety is my passion.
        memcpy(buf, path, pathlen);
        char* buf_point = buf+pathlen;
        *buf_point = '/'; //needed
        memcpy(buf_point+1,cmd,len);
        //naiive but fast to develop.
        struct stat sb;
        if(check(buf, &sb) == NULL || S_ISDIR(sb.st_mode)){
            memset(buf, 0, sizeof(buf));
            printf("Command not found.\n");
            continue;
        }

        int res = fork();
        if(res == -1){
            //failure
            printf("Failure occured");
            return -1;
        }else if(res == 0){
            //child
            //copy required here, as the applicaion now assumes it is in charge of the memory.
            res = execvp(buf, copy_to_array(argv));
        }else{
            //parent
            wait(NULL);
            return 0;
        }

        if(res == -1){
            return -1;
        }
        //here we fork.
        //if parent, wait on child.
        //then exit
        //If child, get execvp on whatever the application that needs to run is.

    }
    return 0;
}

int execute(const int cmd_id, vector_t* args){
   //this should just run the command directly. If it doesn't exist, return a negative number. success on 0.
   switch (cmd_id)
   {
   case ID_echo:
        echo(args);
        break;
   case ID_pwd:
        pwd();
        break;
   case ID_cd:
        cd(args);
        break;
   case ID_setpath:
        setpath(args);
        break;
   case ID_help:
        help();
        break;
   default:
        
        return execute_external(args);
        break;
   }
   return 0;
}



/**
 * @brief Checks to see if the command is a builtin command, uses the POSIX standard for return codes.
 * 
 * @param command A string that is the command to check.
 * @return int Returns -1 on failure, index of the builtin command otherwise.
 */
const int builtin_command_ID(const char* command){

    for(int i=0; i<__SENTINEL__; i++){
        if(strcmp(command, builtin_commands[i]) == 0){
            return i;
        }
    }
    return -1;
}