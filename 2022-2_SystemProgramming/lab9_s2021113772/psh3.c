/*
    prompting shell version 2

    Solves the 'one-shot' problem of version 1
    Uses execvp(), but fork()s first so that the
    shell waits around to perform another command
    New problem: shell catches signals. Run vi, press ^c.

    1) when the user types the command “exit” or
    2) when the program sees end of file (EOF). (ctrl+d)
    Hint) The changes were added in the main input loop.
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXARGS 20
#define ARGLEN 100

void execute(char **);
char *makestring(char *);

int main()
{
    char *arglist[MAXARGS+1];
    int numargs;
    char argbuf[ARGLEN];
    char *makestring();

    numargs = 0;

    printf("Arg[%d]? ", numargs);
    while (fgets(argbuf, ARGLEN, stdin) && numargs < MAXARGS)
    {
        if (*argbuf != '\n') {
            char *res = makestring(argbuf);
            if (strcmp(res, "exit")==0)
                exit(0);
            arglist[numargs++] = res;
        }
            
        else
        {
            if (numargs > 0)
            {
                arglist[numargs] = NULL;
                execute(arglist);
                numargs = 0;
            }
        }
        printf("Arg[%d]? ", numargs);
    }
    return 0;
}


void execute(char *arglist[])
{
    int pid, exitstatus;

    pid = fork();
    switch (pid)
    {
        case -1:
            perror("fork failed");
            exit(1);
        case 0:
            execvp(arglist[0], arglist);
            perror("execvp failed");
            exit(1);
        default:
            while (wait(&exitstatus) != pid)
                ;
            printf("child exited with status %d, %d\n", exitstatus>>8, exitstatus&0377);
    }
}

char *makestring(char *buf)
{
    char *cp;
    
    buf[strlen(buf)-1] = '\0';
    cp = malloc(strlen(buf)+1);
    if (cp == NULL)
    {
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}