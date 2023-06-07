#include"headers.h"
#include"pwd.h"

extern char curr_dir[size],prev_dir[size], home_dir[size], *username, *hostname;

int pwd(int argc, char *argv[])
{
    printf("%s\n", curr_dir);
}
