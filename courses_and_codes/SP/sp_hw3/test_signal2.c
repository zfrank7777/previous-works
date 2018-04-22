#include <stdio.h>
#include <unistd.h>
#include <signal.h>
void print(int signo)
{
	if(signo==SIGUSR1)
		printf("SIGUSR1");
	else 
		printf("SIGUSR2");
}	
int main()
{
	struct sigaction act;
	act.sa_handler=print;
	act.sa_flags=0;
	if(sigaction(SIGUSR1,&act,NULL)==-1)
		printf("fail to sigusr1");
	if(sigaction(SIGUSR2,&act,NULL)==-1)
		printf("fail to sigusr2");
	pause();	

	return 0;
		
}	
