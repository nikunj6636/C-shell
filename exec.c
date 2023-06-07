#include"headers.h"
#include"exec.h"

extern process pArray[33], fg_process; // background process array
extern int npArray, pTime; // to store the time of the process

time_t start, end;

void fg(int argc, char *argv[])
{
    pid_t pid = fork();
    /* fork creates a child process which is copy of the original process and execvp
    reples the old process with the completely new process*/

    if (pid == -1) // error handling
    {
        perror("fork");
        exit(0); // terminate the program execution
    } 

    else if (pid == 0)  // run the foreground process in the child
    {
        if ( execvp(argv[0], argv) == -1 ){
            fprintf(stderr, "%s: command not found\n",argv[0]); /*If execvp failes*/
            exit(1);
        }
        return;
    } 

    else
    {
        start = time(NULL);

        fg_process.n_cmd = argc;
        for(int i=0; i<argc; i++){
            fg_process.cmd[i] = argv[i]; // copying the commands
        }

        fg_process.pid = pid;

        waitpid(pid, NULL, WUNTRACED); 
        // wait till child terminates / stops

        fg_process.pid = -1; // terminated foreground process
        end = time(NULL);
        int time = (end - start); 
        if (time >= 1) pTime = time;
    }
}

void bg(int argc, char *argv[])
{
    int status;

    pid_t pid = fork();
    if (pid == -1) // error handling
    {
        perror("fork");
        exit(0); // terminate the program execution
    } 

    else if (pid == 0)  // run the foreground process in the child
    {
        setpgrp(); 
        if ( execvp(argv[0], argv) == -1 ){
            fprintf(stderr, "%s: command not found\n",argv[0]);
            exit(1);
        } 
        // the process is completedly replaced by new process by execvp command
    } 

    else // print pid on shell when the background process starts
    {  
        process P; 
        for(int i=0; i<argc; i++){
            P.cmd[i] = argv[i]; // copying the commands
        }

        P.n_cmd = argc;
        P.pid = pid;

        if (npArray == 0) P.s = 1;
        else P.s = pArray[npArray].s + 1; // index of the process

        pArray[++npArray] = P; // store in the processes

        printf("[%d] %d\n", P.s, P.pid); // print the process number and process id.
    }
}

void exec(int argc, char *argv[])
{
    int nptr = 0;
    char *ptr[128]; // array of pointer

    for(int i=0; i<argc; i++) // whenver you hit & stop
    {
        if (strcmp(argv[i], "&") != 0)
        {
            if ( argv[i][strlen(argv[i]) - 1] == '&'){

                argv[i][strlen(argv[i]) - 1] = '\0'; // null char
                ptr[nptr++] = argv[i]; 

                // call for background processes
                ptr[nptr] = NULL;
                bg(nptr, ptr); 
                nptr = 0;
            }

            else ptr[nptr++] = argv[i]; // putting the command there
        }

        else{
            ptr[nptr] = NULL; // put NULL at end for calling execvp command
            bg(nptr, ptr); // call for background processes
            nptr = 0;
        }
    }

    if (nptr != 0){
        ptr[nptr] = NULL;
        fg(nptr, ptr); // calling for the foreground process, using the argc, argv technique.
    }
}