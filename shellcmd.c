#include "shellcmd.h"

void redirect( char * outputFile)
{
    int frd = creat(outputFile,0640);
    close(1);
    dup(frd);
    close(frd);
}


/*
    this function redirects data from a file
*/
void FileInput(const char * infile)
{
    int frd;
    int ifile = open(infile, O_RDONLY);
    if(ifile<0)
    {
        perror("Failed to load File\n");
        printf("\nFailed to load File %s %d \n",infile,ifile);
    }
    close(0);
    dup(ifile);
    close(ifile);
}







//this handles setting up to rad from a pipe
void ReadPipe( int fd[2])
{
    close(fd[1]);
    int d =dup2(fd[0], STDIN_FILENO);
    if( d<0)
    {
        perror("Failed to read Pipe");
        fprintf(stderr,"Failed to dup = %d\n\n",d);
        exit(1);
    }
}




/*
    currFROMapp  - index in array of commands that will be FROm in this proc
    NumPipes     - total number of Pipes
    *apps        - list of apps with 
    fd[2]        - pipe values
*/
int pipe_proc(int currFROMapp, int NumPipes,  char *apps[], int fd[2])
{
     
    char * infile;
    bool redir =false;
    if( currFROMapp==0)
    {
        int inIdx =NumPipes-1;
        if (inIdx<0)
        {
            inIdx=0;
        }
        bool binfile =hasInputRedir(apps[inIdx]);
        if(binfile)
        {
            bool SockIN = IsSockRedir(apps[inIdx]);
            if( SockIN)
            {
                // get port number  port num is rest of data in string after @
                // find @ then get next
                char * portStr = strchr(apps[inIdx], '@');
                int portNum = atoi( (portStr+1));
                socketInput(portNum);
            }
            else
            {
                infile = infileRedir(apps[inIdx]);
                if( strlen(infile) >0   )
                {
                    FileInput(infile);
                }
            }
        }
    }
    
    char * trimmed = rtrim(ltrim(apps[currFROMapp]));
    
    int prmcnt = cntParams(trimmed);
     
    char ** cmdPrms;
     
    cmdPrms = allocateArr(prmcnt,cmdPrms,prmsz);
     
    int prmret = SplitPrms(cmdPrms ,trimmed);
 
    int pipeStat =0;
    int status;
    pid_t pid;
    bool last =false;
    if(currFROMapp>=NumPipes){
        last=true;
    }
     
    if(currFROMapp>0)
    {
        ReadPipe(fd);
    }
    if(!last)
    {
        pipeStat = pipe(fd);
        if( pipeStat<0)
        {
            perror("Error creating pipe");
            exit(EXIT_FAILURE);
        }
    }
    pid = fork();
    if(pid<0)
    {
        perror("Error creating FORK");
        exit(EXIT_FAILURE);
    }
     
    if( pid==0)
    {
        if(!last)
        {
            pipeouput(fd);
        }
        redir =hasOutputRedir(trimmed);
        if(last&&redir)
        {
            if( hasOutputRedir(trimmed))
            {
                char * first = strchr(trimmed,'@');
                ++first;
                // get port number
                int portno =0;
                char * sportno = strrchr(trimmed, ':');
                portno = atoi( sportno+1 );
                // get server address.
                char * servAddress =(char*) calloc(( sportno-first+1    ),sizeof(char)    );
                strncpy(servAddress,first, (sportno-first));
                redirectToSocket(portno,servAddress);
                free (servAddress);
            }
            else
            {
                redirect( outfileRedir(trimmed));
            }
        }
        int status =execvp(cmdPrms[0], cmdPrms );
        perror("execvp");
        fprintf(stderr,"Failed to run %s %d\n",cmdPrms[0], status);
        exit (EXIT_FAILURE);
    }else
    {
        
        pid = wait(&status);
        if(!last)
        {
            currFROMapp++;
            pipe_proc(currFROMapp, NumPipes, apps, fd);
        }
        return 0;
    }
}

void pipeouput (int fd[2])
{
    close (fd[0]);
    int fdesc =dup2(fd[1], STDOUT_FILENO);
    if(fdesc<0)
    {
        perror("pipeouput");
        fprintf(stderr, "pipeouput %d", fdesc);
        exit(EXIT_FAILURE);
    }
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}