#include"headers.h"
#include"pinfo.h"

extern char home_dir[size];

void pinfo(int argc, char *argv[])
{
    if (argc > 2){
        printf("pinfo: too many arguements\n");
        return;
    }

    char filename[size] = "/proc/";

    if (argc == 1){
        strcat(filename, "self"); // to get the status of current shell program
    }
    else{
        strcat(filename, argv[1]); // to get the status of the process id
    }

    char statfile[size];
    strcpy(statfile, filename);
    strcat(statfile, "/stat");

    int pid, memory, pgrp, tpgid;
    char status, dummy[size];

    FILE *fp = fopen(statfile, "r");
    if (fp == NULL){
        perror("statfile");
        return;
    }

    else{
        fscanf(fp, "%d %s %c %s %d %s %s %d", 
        &pid, dummy, &status, dummy, &pgrp, dummy, dummy, &tpgid);

        for(int i=0; i<14; i++){
            fscanf(fp, "%s", dummy);
        }

        fscanf(fp, "%d", &memory);  // size of the virtual mempory
        fclose(fp);
    }

    printf("pid : %d \nprocess Status : %c%c \n memory : %d {Virtual Memory}\n", pid, status, (pgrp == tpgid) ? '+' : ' ', memory);

    char execfile[size];
    strcpy(execfile, filename);
    strcat(execfile, "/exe\0");


    char path[size];
    ssize_t length = readlink(execfile, path, size);
    path[length] = '\0';
    
    if (length > 0)
    {
        if ( strncmp(home_dir, path, strlen(home_dir)) == 0 )
        {
            char dir[size]="~";
            strcat(dir, path + strlen(home_dir));
            printf("executable path : %s\n",dir);
        }

        else printf("executable path : %s\n", path);
    }

    else{
        perror("execfile");
    }
}