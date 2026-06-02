# Wash Shell

This shell is written in C or C++.
This shell targets a Linux based distribution, namely Ubuntu 24.04.

# Structure

This is the overview of the program. It is named "wash", it is a combination of "Washington" and "shell" namely the first two letters of each come together to spell "wash".

The goal is to be a command line shell, written in C and C++ and targets Linux based distributions, namely; Debian and Ubuntu.

# What is a Shell?

A shell interpreter is typically called a Command Line Interface (CLI) or a shell for short.

This program continually loops and accepts user input. The CLI then acts on the user's input to execute programs, inbuilt functions, and other options depending on the shell.
The most commonly used shell on Debian based systems is `bash`. 

Here is some high level pseudo code of how a shell operates:
```C
while true
    read user_input // The Parser
    if user_input is a built-in command // The Executor
        handle it
    else if user_input is a program on the path
        fork child process to run the program
        wait for the child process to complete
else
    print error
```

The `wash` shell aims to have some basic functionality similar to other shells, like `bash` and `dash` do.


# Built-In commands

`wash` aims to contains a small set of built in commands to ensure basic functionality.

## Exit

This command is to end the shell process. Given that the shell will loop until terminated, having a built in command to terminate the `wash` shell is required.

- Usage: `exit`

This command has no help. It does not take any inputs. 

## echo

This command will print the given user input to the console/shell that the `wash` instance is running in.

- Usage: `echo [user_input]`

This comamnd returns the user input past the `echo` command, skipping the first set of whitespace until the first alphanumeric text is reached. From there on, it will print the `<user_input>` to the shell.

## pwd

This command will print the current working directory.

- Usage `pwd` 

This command does not take any inputs, and will print to the console the "full" path of the current working directory.

Given the limited space the program ahs to work with, the program will not allow any console inputs or outputs to exceed 256 characters.

if the program exceeds 256 characters it will be truncated.


## cd

This will change the current working directory.
- Usage `cd [dir]`

If no arguments are provided, it will change to the user's home directory
If the optional argument dir is provided it will navigate to the given directory.

## setpath

This will set the PATH in the shell.

- Usage: `setpath <dir>+ `

This command will overwrite the path with the arguements that the shell will recieve under this command.

The path when `wash` launches will contain `/bin`


Multiple folders may be added to the PATH, but they must be space separated.
- Even in this case, the previous values get overriden.


## help

This will list all the built-in commands with short descriptions.
- Usage: help
```C
    "echo <message>: prints \"echo <message>\"",
    "pwd: prints the current working directory",
    "cd <path>: changes the current working directory to <path>",
    "setpath <path> (<path>* ) : sets the executable paths to check for executables.",
    "help: prints out help text for builtin commands.",
    "exit: exits the shell."
```

## Redirection

This shell will allow redirection between programs, allowing the moving of the standard output to a file, and standard error to the `<filename>.error`

A missing filename argument, or multiple arguments is not allowed.
- The command will fail if the aboe is true.


## Other Commands

`wash` will allow the running of executable programs outside of the built in ones.

The command will only be executed if it is specified ini the user's PATH.
They will be searched in the order given (left-to-right)


## Head nine

This will open a given file, and print the first $N$ lines from the file. Default is 9, and thus the name.

- Usage ./head_nine [-h] [-n N] `<file>`

Default is 9 lines if `-n` (or) `-N` is not used.

Otherwise the first N lines will be printed.