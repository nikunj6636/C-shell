#include"headers.h"
#include"prompt.h"

extern char curr_dir[size],prev_dir[size], home_dir[size], *username, *hostname;
extern int pTime;

void prompt()
{
    if ( getcwd(curr_dir, size) == NULL)
    {
        perror("program"); // error in the shell's root
        exit(1);
    }

    char dir[size] = "~";
    if ( strncmp(home_dir, curr_dir, strlen(home_dir)) == 0 ){
        strcat(dir, curr_dir + strlen(home_dir));
    }

    else{
        strcpy(dir, curr_dir); // prompt shows the curr_sir
    }

    if (pTime == -1){
        printf("\033[1;32m%s@%s:\033[1;34m%s\033[0;37m$ ", username, hostname, 
        dir); // absolute path
    }

    else{
        printf("\033[1;32m%s@%s:\033[1;34m%s took %ds\033[0;37m$ ", username, hostname, 
        dir, pTime); // absolute path

        pTime = -1;
    }
}
