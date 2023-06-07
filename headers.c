#include"headers.h"

extern int shell_stdout, shell_stdin; 

void shell_std()
{
    dup2(shell_stdout, STDOUT_FILENO);
    dup2(shell_stdin, STDIN_FILENO);
}