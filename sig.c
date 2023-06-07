#include"headers.h"

extern process pArray[33]; // to store background process
extern int npArray;// nElems in pArray

void sig(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "sig: invalid number of arguments\n");
        return;
    }

    int job_num = argv[1][0] - '0';
    int sig_num = argv[2][0] - '0';

    pid_t pid = -1;
    for(int i=1; i<=npArray; i++)
    {
        if (pArray[i].s == job_num){
            pid = pArray[i].pid;
            break;
        }
    }

    if (pid == -1)
    {
        fprintf(stderr, "sig: invalid job number: %s\n", argv[1]);
        return;
    }

    if (kill(pid, sig_num) < 0) // kill used to send the signal
    {
        perror("sig");
        return;
    }
}