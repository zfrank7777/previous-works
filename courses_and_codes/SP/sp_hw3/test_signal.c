#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sighandler(int);

int main()
{
	printf("before signal\n" );
   	signal(SIGINT, sighandler);
   	printf("after signal\n");

   while(1) 
   {
      printf("Going to sleep for a second...\n");
      sleep(1);
   }

   return(0);
}

void sighandler(int signum)
{
   printf("Caught signal %d, coming out...", signum);
   exit(1);
}