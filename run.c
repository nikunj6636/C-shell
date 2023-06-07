#include"headers.h"
#include"run.h"

extern int shell_stdout, shell_stdin; 

int execute(int argc, char *argv[])
{
    if (redirection(&argc, argv) == 0) return 0;
    
    // arguements functions, argc is atlest 1
    if (strcmp(argv[0], "pwd") == 0)  pwd(argc, argv);

    else if (strcmp(argv[0], "echo") == 0) echo(argc, argv);

    else if (strcmp(argv[0], "cd") == 0) cd(argc, argv);

    else if (strcmp(argv[0], "ls") == 0) ls(argc, argv);
    
    else if (strcmp(argv[0], "history") == 0) history(argc, argv);

    else if (strcmp(argv[0], "discover") == 0) discover(argc, argv);
    
    else if (strcmp(argv[0], "pinfo") == 0) pinfo(argc, argv);

    else if (strcmp(argv[0], "jobs") == 0) jobs(argc, argv);

    else if (strcmp(argv[0], "sig") == 0) sig(argc, argv);

    else if (strcmp(argv[0], "fg") == 0) fg_cmd(argc, argv);

    else if (strcmp(argv[0], "bg") == 0) bg_cmd(argc, argv);

    else exec(argc, argv);

    return 1; // successful execution return 1
}

void run(int argc, char *argv[])
{
    int k = 0; // base index 0

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "|") == 0) // change stdout and run and then change stdin
        {
            int tube[2];
            if (pipe(tube) < 0)
            {
                perror("|");
                return;
            }

            // write to pipe
            if (dup2(tube[1], STDOUT_FILENO) < 0)
            {
                perror("|");
                return;
            }

            if ( execute(i-k, argv+k) == 0){ // error handling
                shell_std(); 
                return;
            }

            dup2(shell_stdout, STDOUT_FILENO); // revert back to std_out
            close(tube[1]);

            // read from pipe
            if (dup2(tube[0], STDIN_FILENO) < 0)
            {
                perror("|");
                return;
            }

            close(tube[0]);
            k = i+1; // start from here
        }
    }
    execute(argc-k , argv+k);
    shell_std();
}