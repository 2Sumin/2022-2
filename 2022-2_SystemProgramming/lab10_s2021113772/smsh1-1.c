#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "smsh.h"

#define DFL_PROMPT ">"

int main()
{
    char *cmdline, *prompt, **arglist, **ori_arg;
    int result;
    void setup();

    prompt = DFL_PROMPT;
    setup();

    while ( (cmdline = next_cmd(prompt, stdin)) != NULL ) {
        // ; 찾아서 strtok으로 나누기
        char *token = strtok(cmdline, ";");
        while (token != NULL) {
            // ;로 나눈 것 arglist에 저장
            arglist = splitline(token);

            result = execute(arglist);
            freelist(arglist);
           
            // 다음 ;로 나눈 것
            token = strtok(NULL, ";");
        }
        // 입력받은 것 free
        free(cmdline);
    }
    
    return 0;
}

void setup()
{
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}
