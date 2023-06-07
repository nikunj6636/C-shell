#include"headers.h"
#include"cd.h"

extern char curr_dir[size],prev_dir[size], home_dir[size], *username, *hostname;

int cd(int argc, char *argv[])
{
    if (argc > 2){
        printf("cd: too many arguments\n");
        return -1;
    }

    if (argc == 1) 
    {
        if (chdir(home_dir) < 0){
            perror("program");
            return -1;
        }
    }
    // no arguement passed to cd

    else if (strncmp("~", argv[1], 1) == 0)
    {
        char dir[size];
        sprintf(dir, "%s%s", home_dir, argv[1]+1);

        if (chdir(dir) < 0) // using only absolute paths in computation
        {
            perror("program");
            return -1;
        }
    }
    
    else if (strcmp(argv[1], "-") == 0){
        
        if (chdir(prev_dir) < 0) {
            perror("program");
            return -1;
        }

        else printf("%s\n", prev_dir); // printing absolute path of prev directory
    }

    else{
        if (chdir(argv[1]) < 0)
        {
            perror("program");
            return -1;
        }
    }

    strcpy(prev_dir, curr_dir); // curr_dir becomes the prev_dir
    getcwd(curr_dir, size); // updating the curr_dir
}
