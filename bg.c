#include"headers.h"
#include"bg.h"

extern process pArray[33]; // to store background process
extern int npArray;// nElems in pArray

void bg_cmd(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "bg: invalid number of arguments\n");
        return;
    }

    int job_num = argv[1][0]- '0';

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
        fprintf(stderr, "bg: invalid job number: %s\n", argv[1]);
        return;
    }

    if (kill(pid, SIGCONT) < 0)
    {
        perror("bg");
        return;
    }
}
