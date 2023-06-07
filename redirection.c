#include"headers.h"
#include"redirection.h"

int redirection(int *argc, char *argv[])
{
    for(int i = 0; i < *argc; i++)
    {   
        // for output file stream : 

        if (argv[i][0] == '>')
        {   
            if (i == *argc-1){
                fprintf(stderr, "%s : error expected arguement after %s", argv[0], argv[i]);
                return 0;
            }

            int fd = -1;
            if (strcmp(argv[i], ">") == 0) // CASE 1
            {
                fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                
                if (fd < 0)
                {
                    perror("outstream");
                    return 0;
                }
            }

            else if (strcmp(argv[i], ">>") == 0) // CASE 2
            {
                fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd < 0)
                {
                    perror("outstream");
                    return 0;
                }
            }

            else{
                fprintf(stderr,"invalid symbol : %s", argv[i]);
                return 0;
            }

            if (dup2(fd, STDOUT_FILENO) < 0) // wrtiting stdout in file
            {
                perror("outstream"); 
                close(fd);
                return 0;
            } 
            
            close(fd); // close the file
        }

        // for input file stream : , both inut and ouptut can be present 

        if (argv[i][0] == '<')
        {    
            if (strcmp(argv[i], "<") != 0){
                fprintf(stderr,"invalid symbol : %s", argv[i]);
                return 0;
            }

            if (i == *argc-1){
                fprintf(stderr, "%s : error expected arguement after %s", argv[0], argv[i]);
                return 0;
            }

            int fd = open(argv[i + 1], O_RDONLY, 0); // read only mode
            if (fd < 0){
                fprintf(stderr, "input file %s don't exist\n", argv[i+1]);
                return 0;
            }   

            if (dup2(fd, STDIN_FILENO) < 0) 
            {
                perror("instream");
                close(fd);
                return 0;
            }

            close(fd);
        }
    }

    // considering valid arguements : only single input / output file

    int steps = 0;
    for(int i = 0; i < *argc; i++)
    {
        if (argv[i][0] == '>' || argv[i][0] == '<'){
            i++; // ignore filename
            steps += 2;
        }

        else argv[i-steps] = argv[i];
    }

    *argc -= steps; // change the no. of commands
    return 1;
}