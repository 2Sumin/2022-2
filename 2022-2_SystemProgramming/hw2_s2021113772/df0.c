#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/statfs.h>
#include <string.h>
#include <unistd.h>
static int ok = EXIT_SUCCESS;

// 바이트로 나타낸 값들
unsigned long blocks = 0;
unsigned long used = 0;
unsigned long avail = 0;
long long percent = 0;

// 단위 숫자로 표현
int mode = 0;

// 출력 안내문
int print_int = 2;

// 1K-blocks    Used Available MyUse% 
void print_units();
void df(char *s, int always);

int main(int argc, char *argv[]) 
{
    // 옵션 붙으면 argc = 3, 없으면 2
    // argv[0] : ./df0
    // argv[1] : .
    // argv[2] : -k / -g / -b / -m

    if (argc == 1) {
        perror("no directory");
        ok = EXIT_FAILURE;
        exit(ok);
    }

    if (argc == 2) //default
        print_int = 2;
  
    else {
        char *unit=strtok(argv[2], "-");
        switch(unit[0]) {
            case 'b':
            case 'B':
                print_int = 1;
                break;
            case 'k':
            case 'K':
                print_int = 2;
                break;
            case 'm':
            case 'M':
                print_int = 3;
                break;
            case 'g':
            case 'G':
                print_int = 4;
                break;
            default: 
                break;
        }
    }
    print_units();

    df(argv[1], 0);
    exit(ok);
}

void print_units()
{
    switch(print_int) {
        case 1: //b
            mode = 'b';
            printf("1B-blocks\tUsed\t\tAvailable\tMyUse%%\n");
            break;
        case 2: //k
            mode = 'k';
            printf("1K-blocks\tUsed\t\tAvailable\tMyUse%%\n");
            break;
        case 3: //m
            mode = 'm';
            printf("1M-blocks\tUsed\t\tAvailable\tMyUse%%\n");
            break;
        case 4: //g
            mode = 'g';
            printf("1G-blocks\tUsed\t\tAvailable\tMyUse%%\n");
            break;
        default:
            break;
    }
}
/*
fsblkcnt_t f_blocks;   Total data blocks in filesystem 
fsblkcnt_t f_bfree;   Free blocks in filesystem 
fsblkcnt_t f_bavail;  Free blocks available to unprivileged user 
*/
void df(char *s, int always) {
    struct statfs st;
    if (statfs(s, &st) < 0) {
        fprintf(stderr, "%s: %s\n", s, strerror(errno));
        ok = EXIT_FAILURE;
    } else {
        if (st.f_blocks == 0 && !always)
            return;    
        // 바이트 단위로 모두 계산  
        // 1B-blocks
        blocks = st.f_blocks*st.f_bsize;
        // Used
        used = (st.f_blocks-st.f_bfree)*st.f_bsize;
        // Available
        avail = st.f_bavail*st.f_bsize;
        // MyUse%
        percent = 100 - (long long)st.f_bavail * 100 / (long long)st.f_blocks;

        // 단위 환산
        switch(mode) {
            case 'b':
                break;
            case 'k': //1KB = 1024B
                blocks /= 1024;
                used /= 1024;
                avail /= 1024;
                break;
            case 'm': //1MB = 1024KB = 1024 * 1024B
                blocks /= 1024 * 1024;
                used /= 1024 * 1024;
                avail /= 1024 * 1024;
                break;
            case 'g': //1GB = 1024MB = 1024 * 1024 * 1024B
                blocks /= 1024 * 1024 * 1024;
                used /= 1024 * 1024 * 1024;
                avail /= 1024 * 1024 * 1024;
                break;
            default:
                break;
        }

        printf("%-15lu\t%-15lu\t%-15lu\t%4lld%%\n", blocks, used, avail,percent);
    }
}
