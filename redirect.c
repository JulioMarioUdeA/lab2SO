#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *arg[])
{
    int pid = fork();
    if (pid < 0)
    {
        printf("Error launching the process\n");
    }
    else if (pid == 0)
    {
        char *myargs[2];
        myargs[0] = "cat";
        myargs[2] = NULL;

        int err = open("cerr.log", O_RDWR | O_CREAT | O_APPEND, 0600);
        if (-1 == err)
        {
            perror("opening cerr.log");
            return 255;
        }

        if (-1 == dup2(err, fileno(stderr)))
        {
            perror("cannot redirect stderr");
            return 255;
        }

        execvp(myargs[0], myargs);
    }
    else
    {
        wait(NULL);
        printf("I am the father and I print in the console\n");
    }
}