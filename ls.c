#include"headers.h"
#include"ls.h"

extern char curr_dir[size],prev_dir[size], home_dir[size], *username, *hostname;

int cmp(const void *a, const void *b){
    return strcmp((char*)a, (char*)b);
}

int print_long(char *dir) // print in long format
{
    struct stat stats;
    if (stat(dir, &stats) != 0)  // error handling
    {
        perror("stats");
        return -1;
    } 

    // directory
    printf(S_ISDIR(stats.st_mode) ? "d" : "-");

    // user permissions
    printf(stats.st_mode & S_IRUSR  ? "r" : "-");
    printf( stats.st_mode & S_IWUSR? "w" : "-");
    printf(stats.st_mode & S_IXUSR? "x" : "-");

    // group permissions
    printf(stats.st_mode & S_IRGRP  ? "r" : "-");
    printf( stats.st_mode & S_IWGRP? "w" : "-");
    printf(stats.st_mode & S_IXGRP? "x" : "-");

    // other permissions
    printf(stats.st_mode & S_IROTH  ? "r" : "-");
    printf( stats.st_mode & S_IWOTH? "w" : "-");
    printf(stats.st_mode & S_IXOTH? "x" : "-");

    struct passwd *uid = getpwuid(stats.st_uid); ; // user id 
    struct group *gid = getgrgid(stats.st_gid);; // group id 
    int n_link = stats.st_nlink;

    printf(" %d %s %s ",n_link, uid->pw_name, gid->gr_name);  // print user id 

    //file size
    printf("%ld ", stats.st_size);

    // local time 
    struct tm *time;
    time_t t = stats.st_mtime;
    time = localtime(&t);   

    if(time == NULL) { // error handling
        perror("localtime"); 
    } 

    char str[size];
    strftime(str, size, "%b %d %R", time); 
    printf("%s ", str);
}

void print_file(char *file, bool l_flag, bool a_flag) // passing paths to file and folder
{
    struct stat stats;
    stat(file, &stats);      
     
    if (l_flag == true)
    {
        print_long(file);
    }
    
    // executable - green
    if (stats.st_mode & S_IXUSR) printf("\033[0;32m%s\n", file);

    // file -white
    else printf("\033[0;37m%s\n", file);
}


int print_folder(char *folder, bool l_flag, bool a_flag, bool multiple) // passing path to folder
{
    // list all files in the folder
    if (multiple == true){
        printf("%s:\n", folder);
    }

    struct dirent **namelist;
    int n = scandir(folder, &namelist, NULL, alphasort); // path of the folder is used here.
    if (n == -1) {
        perror("scandir");
        return -1;
    }

    if (l_flag == true)
    {   
        int total = 0;
        for(int i=0; i<n; i++)
        {
            if (a_flag == false && (namelist[i]->d_name[0] == '.')) continue;

            struct stat stats; // stat library
            char dir[size];
            sprintf(dir, "%s/%s", folder, namelist[i]->d_name); 
            // dir is the path of the file/folder.

            stat(dir, &stats);
            total += stats.st_blocks; 
        }
        printf("total %d\n", total/2);
    }


    for(int i=0; i<n; i++)
    {
        if (a_flag == false && (namelist[i]->d_name[0] == '.')) continue;

        struct stat stats; // stat library
        char dir[size];
        sprintf(dir, "%s/%s", folder, namelist[i]->d_name); 
        // dir is the path of the file/folder.

        stat(dir, &stats);

        if (l_flag)
        {
            print_long(dir);
        }

        // folder - blue color
        if (S_ISDIR(stats.st_mode)) printf("\033[0;34m%s\n",namelist[i]->d_name);
        
        // executable - green, filewith executable permissions
        else if (stats.st_mode & S_IXUSR) printf("\033[0;32m%s\n",namelist[i]->d_name);

        // file -white
        else printf("\033[0;37m%s\n",namelist[i]->d_name);
        
        printf("\033[0;37m"); // white color
    } 
}


void ls(int argc, char *argv[])
{
    bool l_flag = false, a_flag = false;

    int n_file=0;
    char *file[size]; // pointer to file names

    int n_folder=0;
    char *folder[size]; 
    bool invalid_dir = false;

    for(int i=1; i<argc; i++)
    {
        if (strlen(argv[i]) != 1 && argv[i][0] == '-'){
            for(int j=1; argv[i][j]!='\0'; j++)
            {   
                if (argv[i][j] == 'a') a_flag = true;
                else if (argv[i][j] == 'l') l_flag = true;
                else{
                    printf("ls: invalid option -- %c\n", argv[i][j]);
                    return;
                }
            }   
        }

        else{ // file or directory
            char *dir = (char*)malloc(sizeof(char)*size);
            if (strncmp("~", argv[i], 1) == 0) sprintf(dir, "%s%s", home_dir, argv[i]+1);
            else strcpy(dir, argv[i]);

            struct stat stats; // stat library
            if (stat(dir, &stats) == 0) // file exixts at return value 0
            {
                if (S_ISDIR(stats.st_mode)) 
                {
                    folder[n_folder++] = dir;
                }

                else{
                    file[n_file++] = dir;
                }
            }
            else{ // file does not exist
                printf("ls: %s - No such file or directory\n", argv[i]);
                invalid_dir = true;
            }
        }
    }

    qsort(file, n_file, sizeof(char*), cmp);
    qsort(folder, n_folder, sizeof(char*), cmp);
    
    if (n_file == 0 && n_folder==0 && invalid_dir==false){
        folder[n_folder++] = curr_dir; // contains the address of the curr directory
    }

    for(int i=0; i<n_file; i++)
    {
        print_file(file[i], l_flag, a_flag);
    }
    if (n_file > 0) printf("\n");

    bool multiple = false;
    if (n_file + n_folder > 1) multiple = true;

    for(int i=0; i<n_folder; i++) // using paths to file and folders
    { 
        print_folder(folder[i], l_flag, a_flag, multiple);
        if (i != n_folder-1)printf("\n");
    }
}