#include"headers.h"
#include"signal.h"

extern process pArray[33], fg_process; // to store background process
extern int npArray;

extern char *exit_process[32]; 
extern int nexit;

void bg_handler()
{
    int status; // to consider the status of the child process
    pid_t pid = waitpid(WAIT_ANY, &status, WNOHANG); // returns pid of the terminated process

    for (int i = 1; i <= npArray; i++)
    {
        if (pArray[i].pid == pid) // process terminated
        {
            char exit_status[32];

            if (WIFEXITED(status) == true) strcpy(exit_status, "normally");
            else strcpy(exit_status, "abnormally");

            exit_process[++nexit] = (char*)malloc(sizeof(char)*size);
            sprintf(exit_process[nexit], "%s with pid %d exited %s",  pArray[i].cmd[0], pArray[i].pid, exit_status);

            for (int j = i; j < npArray; j++) pArray[j] = pArray[j + 1]; // shift left
            npArray--;

            break;
        }
    }
}

void c_handler()
{
    return;
}

void z_handler()
{
    if (fg_process.pid != -1) // running foreground process
    {    
        if (npArray == 0) fg_process.s = 1;
        else fg_process.s = pArray[npArray].s + 1; // index of the process

        pArray[++npArray] = fg_process; // store in background process

        if (kill(fg_process.pid, SIGTSTP) < 0){
            perror("SIGTSTP");
        }

        else{
            printf("\nStopped %s and pushed it to background\n", fg_process.cmd[0]);
            fg_process.pid = -1;
        }
    }
}

