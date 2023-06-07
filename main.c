#include"headers.h"
#include"run.h"

char curr_dir[size], prev_dir[size], home_dir[size], *username, *hostname; // absolute paths only

process pArray[33], fg_process; // to store background process

int npArray = 0, pTime = -1; // nElems in pArray
// processes are 1 based indexing

int shell_stdout, shell_stdin; // stdin and stdout

pid_t shell_pid;

char *exit_process[32]; int nexit = 0;

void print_exit_process()
{
    for(int i=1; i <= nexit; i++)
    {
        printf("%s\n", exit_process[i]);
        free(exit_process[i]);
    }
    nexit = 0;
}

int main()
{
    username = (char*)malloc(size*sizeof(char));
    username = getlogin();

    hostname = (char*)malloc(size*sizeof(char));
    if ( gethostname(hostname, size) < 0){
        perror("program");
        exit(0);
    }

    // home directory of the shell is ~
    if ( getcwd(home_dir, size) == NULL)
    {
        perror("program"); // error in the shell's root
        exit(1);
    }
    strcpy(prev_dir, home_dir); // making the prev dir home dir
    strcpy(curr_dir, home_dir);

    // read history.txt
    read_logs();

    // create copy of stdin and stdout files 
    shell_stdout = dup(STDOUT_FILENO);
    shell_stdin = dup(STDIN_FILENO);

    // to get shell's pid
    shell_pid = getpid();

    // handling signals
    signal(SIGCHLD, bg_handler);
    signal(SIGINT, c_handler);
    signal(SIGTSTP, z_handler);

    fg_process.pid = -1; // no fg_process running
    // ctrl + z, currently running foreground process is stopped and pushed in the background
 
    // infinite loop of shell
    while(1)
    {   
        print_exit_process(); // to print the exited backgound process

        prompt();

        // taking the input commands 
        char *cmd = autocomplete();

        // updating logs
        char *logs = (char*)malloc(sizeof(char)*size);
        strcpy(logs, cmd);
        update_logs(logs); 

        // tokenize along ;

        char *commands[32];
        int cmd_num = 0;

        char *tokenize = strtok(cmd, ";"); // to consider the foreground processes
        while (tokenize != NULL) // NOTE: fgets reads the new line char
        {
            commands[cmd_num++] = tokenize;
            tokenize = strtok(NULL, ";");
        }

        for(int i=0; i<cmd_num; i++) // execute all commands separated by ';'
        {
            tokenize = strtok(commands[i], " \t\n"); // to consider the tab and space character
            char *argv[128]; // array of pointers to the instructions
            int argc = 0;

            if (tokenize == NULL) continue; // empty command

            while (tokenize != NULL) // NOTE: fgets reads the new line char
            {
                argv[argc++] = tokenize;
                tokenize = strtok(NULL, " \t\n");
            }

            run(argc, argv); // argc is 0 based indexing
        }
    }   
}