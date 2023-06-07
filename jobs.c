#include"headers.h"
#include"jobs.h"

extern process pArray[33]; // to store background process
extern int npArray; // nElems in pArray

typedef struct pstatus{
    process P;
    int status; // 0 : stopped, 1 : running
}pstatus;

int cmpprocess(const void* a, const void* b)
{
    return strcmp( ((pstatus *)a)->P.cmd[0], ((pstatus *)b)->P.cmd[0]);
}

void display_jobs(bool r_flag, bool s_flag)
{
    // go over all jobs print and sort them, use pointers to all jobs
    pstatus sorted[32];
    int count = 0; 

    for(int i=1; i<=npArray; i++) // 1 based indexing, all background processes
    {
        char path[size];
        sprintf(path, "/proc/%d/stat", pArray[i].pid);

        FILE *fp = fopen(path, "r");
        if (fp == NULL)
        {
            perror("/proc/stat");
            return;
        }

        else{
            char dummy[32];
            char c;
            fscanf(fp, "%s %s %c", dummy, dummy, &c);

            if (c == 'T' && s_flag == true){ // stopped process
                sorted[++count].P = pArray[i];
                sorted[count].status = 0;
            }

            else if (r_flag == true){ // running process
                sorted[++count].P = pArray[i];
                sorted[count].status = 1;
            }

            fclose(fp);
        }
    }

    qsort(sorted+1, count, sizeof(pstatus), cmpprocess); // 1 based indexing
    
    // index , status, name , pid
    for(int i=1; i <= count; i++)
    {
        printf("[%d] %s", sorted[i].P.s, (sorted[i].status == 0) ? "Stopped " : "Running ");

        for(int j=0; j<sorted[i].P.n_cmd; j++){
            printf("%s ", sorted[i].P.cmd[j]);
        }

        printf("[%d]\n", sorted[i].P.pid);
    }
}

void jobs(int argc, char *argv[])
{
    bool s_flag = false, r_flag = false;
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-r") == 0) r_flag = true;

            else if (strcmp(argv[i], "-s") == 0) s_flag = true;
           
            else
            {
                fprintf(stderr, "invalid flag: %s\n", argv[i]+1);
                return;
            }
        }
    }

    else r_flag = true;  s_flag = true;
    
    display_jobs(r_flag, s_flag);
}