#include"headers.h"
#include"history.h"
#include"queue.h"

extern char home_dir[size];
char *prev_history;
Queue Q;

void history(int argc, char *argv[])
{
    if (argc > 1)
    {
        printf("history: Too many arguments\n");
        return;
    }
    PrintQueue(Q); // print atmost 10 elems, use without '\n' char
}

void read_logs()
{
    char log_file[buff_size];
    sprintf(log_file, "%s/.history", home_dir); // path of shell_logs

    FILE *fp;
    fp = fopen(log_file, "r"); // open in the read mode

    Q = QueueInit(20);

    if (fp == NULL) // create the file if it dont exist
    {
        fp = fopen(log_file, "w"); 
        prev_history = NULL;
    }
    
    else
    {
        // reads the string until encounters an EOF
        while (true) 
        {
            char* cmd = (char*)malloc(sizeof(char)*size);
            if (fgets(cmd, size, fp) == NULL) break;
            char *tokenize = strtok(cmd, "\n"); // remove \n and point to the first char of the string

            EnQueue(Q, cmd);
            prev_history = cmd;
        }
    }
    fclose(fp);
}

void write_logs()
{
    char log_file[buff_size];
    sprintf(log_file, "%s/.history", home_dir); // path of shell_logs

    FILE *fp;
    fp = fopen(log_file, "w"); // open in the read mode

    if (fp == NULL) 
    {
        perror(".history"); // file does not exist
        return;
    }

    else
    {
        while(!isQueEmpty(Q)) 
        {
            fprintf(fp, "%s", DeQueue(Q));
            if (!isQueEmpty(Q)) fprintf(fp, "%s", "\n");
        }
        fclose(fp);
    }
}

// cmd is malloced
void update_logs(char *cmd) // passing the pointer of the string
{
    char command[size]; // temporary memory
    strcpy(command, cmd);

    char *tokenize = strtok(command, " \t\n");
    if (tokenize == NULL) return;

    tokenize = strtok(cmd, "\n");
    
    if (prev_history != NULL && strcmp(cmd, prev_history)==0) return;
    
    if (isQueFull(Q)) DeQueue(Q);
    
    EnQueue(Q, cmd);
    prev_history = cmd;
}