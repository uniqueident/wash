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

//Builtin commands to read.
const char* builtin_commands[] = {
    "echo",
    "pwd",
    "cd",
    "setpath",
    "help",
    "exit"
};

//Flavor text for the help command.
const char* help_text[] ={
    "echo <message>: prints \"echo <message>\"",
    "pwd: prints the current working directory",
    "cd <path>: changes the current working directory to <path>",
    "setpath <path> (<path>* ) : sets the executable paths to check for executables.",
    "help: prints out help text for builtin commands.",
    "exit: exits the shell."
};

//Couldn't make string enums in C, so this was a compromise.
typedef enum{
 ID_echo =0 ,
 ID_pwd,
 ID_cd,
 ID_setpath,
 ID_help,
 ID_exit,
 __SENTINEL__
} Builtin;


/**
 * @brief Utility function to get the current path vector.
 * 
 * @return vector_t 
 */
vector_t get_path(){
    return executable_paths;
}

//read the .h file for details.
void create_paths_vector(){
    if(executable_paths == NULL){
        executable_paths = vec_create();
        char* initial = malloc(sizeof(char)*strlen("/bin"));
        char init[] = "/bin";
        memcpy(initial,init,sizeof(init));
        append(&executable_paths,initial);
    }
}
//read the .h file for details.
void destroy_paths_vector(){
    destroy(&executable_paths);
}

/**
 * @brief echos a given message, but since the text has been parsed, spaces and tabs are not kept.
 * 
 * @param message 
 */
void echo(vector_t* message){
    if(message == NULL || vec_size(message)<2){
        printf("Command <%s> requires at least one argument!\n",get(message,0));
        return;
    }
    for(int i = 1; i <vec_size(message)-1; i++){
        printf("%s ",get(message,i));
    }
    printf("%s\n", get(message,vec_size(message)-1));
}

/**
 * @brief Prints the current working directory of the shell.
 * 
 */
void pwd(){
    char cwd[MAX_SIZE+1] = {0};
    char* res = getcwd(cwd, sizeof(cwd));
    if(res == NULL){
        printf( "Unable to write current working directory to buffer\n");
    } else {
        printf("%s\n", cwd);
    }
}

/**
 * @brief Functionally identical to the standard cd command.
 If no arguments are provided, it will attempt to navigate to the home directory.
 If there is no home directory, it will print an error and do nothing.
 If one argument is provided, it will attempt to navigate to that directory.
 If more than one argument is provided, it will do nothing.
 * 
 * @param path The arguments to the cd command.
 * @return int Returns 0 on success -1 on failure.
 */
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
 * @brief Checks to see if given path exists.
 * 
 * @param path Path to check
 * @param sb Stat struct to write the info to.
 * @return struct stat* 
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

/**
 * @brief Sets the executable paths that the shell will utilize to search for executables.
 Any non-directory paths will be ignored.
 If no paths are provided, the path will be cleared, and a warning given.
 * 
 * @param args The paths to set in the path.
 * @return int Returns 0 on success, -1 on failure.
 */
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
        printf("\t%s\n", help_text[i]);
    }
}

/**
 * @brief This is the function to call external commands. It will search the paths for an executable with the same name as the command, and attempt to run it.
 Note that the code is a bit of a mess, but should be safe.
 * 
 * @param argv A vector of arguments for the command
 * @return int Returns 0 on success, -1 on failure.
 */
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

        /*
        This call to fork does not take any arguments,
        This systemcall create a new process by duplicating the calling process.
        In this case: "wash" is the calling process.
        If the return value of the fork is 0 its the new process
        If it is positive, it is the parent of the new process.
        Otherwise, the fork failed.
        */
        int res = fork();
        if(res == -1){
            //failure
            printf("Failure occured");
            return -1;
        }else if(res == 0){
            //child
            /*
            This call to execvp takes the PATH of the executable/script to run and the arguments to provide to said program.
            Now, there is something interesting to note about how I am calling it.
            Since execvp takes an array of char* for its second argument. I use my vector implementation to create said array.
            Now what is notable, is that once that is passed in, the child process now has ownership of said memory.
            As such, the child is now responsible for freeing the array.
            This 
            */
            char** arr = copy_to_array(argv);
            res = execvp(buf, arr);
            if(res == -1){
                free(arr);
                printf("Failure to execute command.\n");
                return -1;
            }
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

/**
 * @brief Runs the builtin command with arguments. If no matching command is found, it will search the given paths for an executable with the same name, and attempt to run it.
 * 
 * @param cmd_id 
 * @param args 
 * @return int 
 */
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

const int builtin_command_ID(const char* command){

    for(int i=0; i<__SENTINEL__; i++){
        if(strcmp(command, builtin_commands[i]) == 0){
            return i;
        }
    }
    return -1;
}