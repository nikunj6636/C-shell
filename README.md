# C-shell
Implemented Bash shell with C language with basic and added functionalities.

# Compiling
execute the given command to create an executable file `make`<br>
Now run<br>
`./a.out`


## Commands and other features

`cd`, `echo`, `exit`, `ls`, `pinfo`, `pwd`, `jobs`, `sig`, `fg`, `bg` and system commands (`vi`, `subl`, etc)

- Piping ( `cmd1 | cmd 2 `  etc.)
- Redirection ( `cmd < input_file >> output_file`  etc.)
- Keyboard interrupts (`Ctrl+C`, `Ctrl+D`, etc.)

For knowing more about how the 'user-defined' commands work (commands which are different from standard bash/zsh commands), you can check Assignment Part1 and 
Part2 for `jobs`, `sig`, `fg` or `bg`.

## Code Files

| Files                 | Description |
| --------------------- | ----------- |
| bg.c, bg.h | Implementing the user defined `bg` command |
| cd.c, cd.h            | Implementing the `cd` command (navigate to a directory). The command takes up to one argument. |
| echo.c, echo.h        | Implementing the `echo` command (printing to the console). Tabs and multiple spaces are handled.        |
| run.c, run.h  | Contains the function for handling all commands and calling the respective functions. **Also implementts piping in the command**        |
| fg.c, fg.g | Implementing the user defined `fg` function.|
| headers.h | Including all external header files          |
| signals.c, signals.h | Contains custom defined signal handler functions. Includes handlers for ^C, ^Z and background processes. |
| jobs.c, jobs.h | Implementation for user defined `jobs` command. Takes the flags `-s`, `-r`, `-sr`, `-rs`. If no flags are given, defaults to `rs` mode. |
| ls.c, ls.h | Implementing `ls` including multiple arguments (both files and directories) and flags `-l` and `-a`.         |
| main.c | The main function of the program. After initializing the shell, it reads input from the user, adds it to history, tokenizes it into commands, and sends it into the `execute` function (`execute.c`)            |
| pinfo.c, pinfo.h      | Implementing the user-defined `pinfo` command. It takes information about the process from `/proc` directory and prints selected information on the terminal.        |
| prompt.c, prompt.h    | Controls the shell prompt, for example, ```<nikunj@nikunj-Inspiron-3501:~$>``` Here, `~` is the directory from which the shell is executed.        |
| pwd.c, pwd.h          | Implements `pwd` command (prints present working directory)        |
| redirection.c, redirection.h | Includes a function called `redirection` which is called from within `execute`. Before the commands are executed, this simplifies the command and reallocates stdin/stdout according to the specified symbols (`>/>>/<`). |       |
| sig.c, sig.h | Implements the user defined `sig` command (`sig [jobnum] [signum]`) |
| exec.c, exec.h    | Implementing system commands using `execvp()`        |
| history.c, history.h    | Implements `history` command( prints previous command executed)        |


## Assumptions

| Feature/String | Limits |
| -------------- | ------------- |
| All paths      | 1024 chars |
| User input     | 256 chars, 32 commands |
| Command arguments        | 32 (excluding flags) |

*This was built as a part of the Operating Systems and Networks course, Monsoon 2022.*
