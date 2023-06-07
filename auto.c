#include "headers.h"
#include"prompt.h"
#include"history.h"

struct termios orig_termios;

extern char curr_dir[size]; // current direcotry

void die(const char *s)
{
    perror(s);
    exit(1);
}

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

char* match_name(char *p) // return pointer to final name
{
    char *folder = NULL, *name = p; // path of the folder, name

    for(int i = strlen(p) - 1; i>=0; i--)
    {
        if (p[i] == '/') // change the directory
        {
            p[i] = '\0';
            folder = p;
            name = p+i+1;
            break;
        }
    }

    char path[2*size];

    if (folder == NULL) strcpy(path, curr_dir);
    else sprintf(path, "%s/%s", curr_dir, folder);
    
    struct dirent **namelist;
    int n = scandir(path, &namelist, NULL, alphasort); // ncontents

    if (n == -1) // error
    {
        perror("scandir");
        return NULL;
    }
    
    int count = 0;
    bool flag = 1;

    char *part = name;

    printf("\n");
    for(int i=0; i<n; i++) 
    {
        if ( strncmp( name, namelist[i]->d_name, strlen(name) ) == 0 ) 
        {
            if (flag == true){
                part = namelist[i]->d_name;
                flag = false;
            }

            else{
                for(int j=0; j<strlen(part); j++)
                {
                    if (namelist[i]->d_name[j] != part[j]) 
                    {
                        part[j] = '\0';
                        break;
                    }
                }
            }

            count++;

            if (folder == NULL) printf("%s", namelist[i]->d_name);

            else printf("%s/%s", folder,namelist[i]->d_name);

            struct stat stats;
            char dir[4*size];
            sprintf(dir, "%s/%s", path, namelist[i]->d_name); // dir is file/folder path
            stat(dir, &stats);

            if (S_ISDIR(stats.st_mode)) printf("/");
            printf("\n");
        }
    }

    char *final = (char*)malloc(sizeof(char) * 100);

    if (count == 1) 
    {
        struct stat stats;
        char dir[4*size];
        sprintf(dir, "%s/%s", path, part); // dir is file/folder path
        stat(dir, &stats);

        if (folder == NULL) strcpy(final, part);
        else sprintf(final, "%s/%s", folder, part);

        if (S_ISDIR(stats.st_mode)) strcat(final, "/");
    }

    else
    {
        if (folder == NULL) strcpy(final, part);

        else sprintf(final, "%s/%s", folder, part);
    } 

    return final;
}

char* autocomplete()
{
    char *inp = (char*)malloc(sizeof(char)*size); // final command is stored in inp
    char c;

    setbuf(stdout, NULL);
    enableRawMode();

    memset(inp, '\0', size);
    int pt = 0; // number of characters stored

    while (read(STDIN_FILENO, &c, 1) == 1) // input taken by 1 characters
    {
        if (iscntrl(c))
        {
            if (c == 10) break;

            else if (c == 27)
            {
                char buf[3];
                buf[2] = 0;
                if (read(STDIN_FILENO, buf, 2) == 2)
                {
                    printf("\rarrow key: %s", buf);
                }
            }

            else if (c == 127)
            {
                if (pt > 0)
                {
                    if (inp[pt - 1] == 9)
                    {
                        for (int i = 0; i < 7; i++)
                        {
                            printf("\b");
                        }
                    }
                    inp[--pt] = '\0';
                    printf("\b \b");
                }
            }
            else if (c == 9) // TAB character here
            {
                for(int i=pt; i>=0; i--)
                {
                    if (inp[i] == ' ')
                    { 
                        inp[i] = '\0'; // break the word, concatenate later
                        
                        char temp[256], *p;
                        strcpy(temp, inp+i+1);

                        p = match_name(temp); // retuns the char after space
                        strcat(inp, " ");
                        strcat(inp, p);
                        pt = strlen(inp); // set pt value 

                        prompt();
                        printf("%s", inp);

                        break;
                    }
                }
            }

            else if (c == 4){ // ctrl + D
                write_logs();
                printf("\n");
                exit(1);
            }

            else{
                printf("%d\n", c);
            }
        }

        else
        {
            inp[pt++] = c;
            printf("%c", c);
        }
    }

    disableRawMode();
    printf("\n");
    return inp;
}