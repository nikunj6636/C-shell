#include"echo.h"
#include"headers.h"

extern char curr_dir[size],prev_dir[size], home_dir[size], *username, *hostname;

void echo(int argc, char *argv[])
{
    for(int i=1; i<argc; i++){
        printf("%s ", argv[i]);
    }
    printf("\n");
}