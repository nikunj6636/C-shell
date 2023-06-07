#include"headers.h"
#include"fg.h"

extern process pArray[33], fg_process; // to store background process
extern int npArray;// nElems in pArray

void fg_cmd(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "fg: invalid number of arguments\n");
        return;
    }

    int job_num = argv[1][0] - '0', index = -1;

    pid_t pid = -1;
    for(int i=1; i<=npArray; i++)
    {
        if (pArray[i].s == job_num){
            pid = pArray[i].pid;
            index = i;
            break;
        }
    }

    if (pid == -1)
    {
        fprintf(stderr, "fg: invalid job number: %s\n", argv[1]);
        return;
    }

    // Terminal I/O for background process
    signal(SIGTTOU, SIG_IGN);  
    signal(SIGTTIN, SIG_IGN); 

    // bring it in the foreground

    fg_process = pArray[index];
    for (int j = index; j < npArray; j++) pArray[j] = pArray[j + 1]; // remove from list
    npArray--;

    tcsetpgrp(STDIN_FILENO, pid);

    if ( kill(pid, SIGCONT) < 0){ 
        perror("SIGTSTP"); // change state to running
    }

    int status;
    waitpid(pid, &status, WUNTRACED); // wait for child process

    tcsetpgrp(STDIN_FILENO, getpgid(0));

    signal(SIGTTOU, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);

    if (WIFSTOPPED(status) == true)
    {
        if (npArray == 0) fg_process.s = 1;
        else fg_process.s = pArray[npArray].s + 1; // index of the process

        pArray[++npArray] = fg_process; // store in background process

        printf("\nStopped %s and pushed it to background\n", fg_process.cmd[0]);
    }  

    fg_process.pid = -1; // terminate the fg_process
}