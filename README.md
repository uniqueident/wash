# Wash Shell (Extra Credit)

This shell is written in C.
This shell targets a Linux based distribution, namely Ubuntu 24.04.

## Synopsis

```sh
wash [-h]
```
The `-h` flag prints the help message and exits immediately.
Without any flags, `wash` launches an interactive session.

## Description

`wash` continually loops and accepts user input. The CLI then acts on the user's input to execute programs, inbuilt functions, and other options depending on the shell.

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
Input is limted to 256 characters per line. Leading and trailing whitespace is ignored for all commands.

The shell starts fresh every run, and does not retain state between sessions.

## Built-In commands

These commands in `wash` are handled directly without forking a child process.

### Exit

This command is to end the shell process. Given that the shell will loop until terminated, having a built in command to terminate the `wash` shell is required.

- Usage: `exit`

This command does not take any arguments.

### echo

This command will print the given user input to the console/shell that the `wash` instance is running in.

- Usage: `echo [message]`

prints everything after `echo` stripping extra whitespace.
### pwd

This command will print the current working directory.

- Usage `pwd` 

This command does not take any arguments, and truncates at 256 characters.


### cd

This will change the current working directory.
- Usage `cd [dir]`

If no arguments are provided, it will change to the user's home directory
If the optional argument dir is provided it will navigate to the given directory.

### setpath

This will set the PATH in the shell.

- Usage: `setpath <dir> (<dir> )* `

This command will overwrite the path with the arguements that the shell will recieve under this command.

The path when `wash` launches will contain `/bin`


Directories are searched left to right when looking for executables.

### help

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

This takes no arguments

### Redirection

Although redirection was planned, due to time constraints, had to be cut. If redirects were to be avaialble in the future, here is what they would do:

This shell will allow redirection between programs, allowing the moving of the standard output to a file, and standard error to the `<filename>.error`

A missing filename argument, or multiple arguments is not allowed.


### Other Commands

`wash` will allow the running of executable programs listed in the current PATH.

The command will only be executed if it is specified in the user's PATH.
By default it only contains `/bin` and will not search the current working directory for executables to run.

Use `setpath` to add and remove directories as needed.

### Head nine

Similar to `head` but defaults to 9 lines

```sh
head_nine [-h] [-n N] [file]
```

- If a `file` is specified prints the first N lines of the file
- If no file is given, it reads from stdin until `Ctrl+D`
- `-n N` sets the number off lines to print
- The order of `[file]` and `[n N]` does not matter.



## How to Compile

Compiling `wash` shell is a fairly simple process on a Debian based system. To ensure that installation remains painless ensure that the following are installed:

- Cmake 3.28 or later
- make
- gcc (C17 or greater)
- gdb
- git (optional)

With Debian/Ubuntu installing `build-essential` and then `cmake` is the simplest to accomplish the above

```sh
sudo apt-get install build-essential cmake
```

Ensure that you are on a posix compliant system as well.

Once completed, it is as easy as placing the following command into your shell.
```sh
cmake -B build && cmake --build build
```

However if that seems like a lot, there is an attached script that does that for you. Ensure it is executable using `chmod +x <script>`, or use the following in your terminal while in the root folder.

```sh
chmod +x ./builder.sh && ./builder.sh
```

Subsequent builds only require:
```sh
./builder.sh
```

## How to use

By running the executable with `wash` it will immediately take you into the shell where the above builtin commands can be used, and any executable within your `/bin` folder, if it exists. Otherwise, add and remove paths with the `setpath` command to ensure that the shell is usable for your system.

Given the limitations of the system, namely its simplicity, it does not keep context across runs. It starts fresh, every single time.

Furthermore, for the sake of making quick development on the system, the entire executable is statically compiled.