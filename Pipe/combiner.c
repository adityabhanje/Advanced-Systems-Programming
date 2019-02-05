#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>


int main(int argc,char* argv[])
{
    int pipefd[2]; 
    pipe(pipefd); 
    int pid1=fork();      
    if (pid1==0)
    {
       close(pipefd[0]);                   
       dup2(pipefd[1], STDOUT_FILENO);
       execvp(argv[1],argv);
    }
    int pid2=fork();
    if (pid2==0)
    {
       close(pipefd[1]);
       dup2(pipefd[0], STDIN_FILENO);
       execvp(argv[2],argv);
       
    }
    close(pipefd[0]);
    close(pipefd[1]);
    exit(0);
}

