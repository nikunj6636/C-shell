#include"headers.h"
#include"discover.h"

extern char curr_dir[size],prev_dir[size], home_dir[size], *username, *hostname;

char *allocate()
{
    return  (char*)malloc(sizeof(char) * size);
}

void printname(char *path) // using path as an arguement
{ 
    if ( strncmp(home_dir, path, strlen(home_dir)) == 0 )
    {
        char dir[size]="~";
        strcat(dir, path + strlen(home_dir));
        printf("%s\n",dir);
    }

    else  printf("%s\n", path);
}

void find(char * path, char *name, bool d_flag, bool f_flag)
{
    DIR * d = opendir(path); 

    if(d == NULL) //erro handling
    {
        perror("opendir");
        return;
    }

    struct dirent * dir; // to read the directory
    while (true) // doing depth first search of the direcotries
    {
        dir = readdir(d); // read the  directory d
        if (dir == NULL) break; // after all dir/files are read

        char f[size];
        sprintf(f, "%s/%s", path, dir->d_name); 
        // f is the absolute path of the dir/file in the given path folder

        if(dir-> d_type != DT_DIR) // it is a file
        {
            if (f_flag == true) // have to search for files
            {
                if (name != NULL){
                    // search for a particlular name
                    if (strcmp(dir->d_name, name) == 0) printname(f);
                }

                else{
                    // don't need to search for any name
                    printname(f);
                }
            }
        }

        else if (dir->d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0)
        {   
            // not a hidden directory
            if (d_flag == true) // have to search for directories.
            {
                if (name != NULL){
                    // search for a particlular name
                    if (strcmp(dir->d_name, name) == 0) printname(f);
                }

                else{
                    // don't need to search for any name
                    printname(f);
                }
            }

            find(f, name, d_flag, f_flag); // recursive search in the direcotory
        }    
    }
    closedir(d); // close the directory
}

// print the path of the name with respect to the target_dir if present
void discover(int argc, char *argv[])
{
    bool d_flag = false, f_flag = false, dir_present = false, name_present = false; 
    // if both flags are false consider both to be true
    // d_flag filters search for directory only and f_flag filters search for file only.

    // target is the target dir_path(without quotes), and in quotes is find "dir/file name" only

    char *target = allocate(), *name = allocate();

    for(int i=1; i<argc; i++) // ordering of arguements does not matter
    {
        // Traversing all the arguements
        if (strlen(argv[i]) != 1 && argv[i][0] == '-'){
            
            if (argv[i][1] == 'd') d_flag = true;
            else if (argv[i][1] == 'f') f_flag = true;
            else{
                printf("discover: invalid option - %s\n", argv[i]);
                return;
            }
        }

        else if (argv[i][0] == '"')
        {
            argv[i][strlen(argv[i]) - 1] = '\0'; // null char, to remove the quotes
            strcpy(name, argv[i]+1);
            name_present = true;
        }

        else{
            
            char *f = allocate();
            if (argv[i][0] == '~') sprintf(f, "%s%s", home_dir, argv[i]+1);
            else strcpy(f, argv[i]);

            struct stat stats;
            if (stat(f, &stats) == 0 && S_ISDIR(stats.st_mode)){ // valid directory
                strcpy(target, f);
                dir_present = true;
            }

            else{
                printf("%s - No such directory\n",argv[i]);
                return;
            }
        }
    }

    if (d_flag == false && f_flag == false){ // flags are same as filters
        d_flag = true;  f_flag = true;
    }

    if (dir_present == false){
        strcpy(target, ".");
    }

    if (name_present == false){
        if (d_flag == true) printname(target); 
        find(target, NULL, d_flag, f_flag);
    }

    else{
        find(target, name, d_flag, f_flag);
    }
}