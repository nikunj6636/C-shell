#ifndef HEADER_H
#define HEADER_H

#include<stdio.h>
#include<fcntl.h> 
#include<sys/types.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<wait.h>
#include<signal.h>
#include <time.h>
#include <termios.h>
#include <ctype.h>

#define size 512
#define buff_size 1024

typedef struct process{
    pid_t pid;
    int s; // sequential index in the terminal
    int n_cmd; 
    char *cmd[size];

}process;

void shell_std();

#endif