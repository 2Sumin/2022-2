
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>


void print_time(int signum);
void quit(int signum);

clock_t start, end;

int main()
{

	start = clock();
	signal(SIGINT, print_time);
	signal(SIGQUIT,quit);

	printf("You can't stop me! \n");
	while(1) {
		// end = clock();
		sleep(1);
		printf("haha\n");
	}
	return 0;
}

void print_time(int signum)
{
	end = clock();
	double duration = (double)(end-start)/CLOCKS_PER_SEC*10000;

	printf("Currently elapsed time: %d sec(s)\n", (int)duration);
}

void quit(int signum)
{
	printf("Quit\n");
	exit(0);
}