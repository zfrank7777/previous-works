#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	FILE* fd=fopen("ans.csv","r");
	FILE* fd2=fopen("submission.csv","r");
	char bug[200];
	fscanf(fd,"%s",bug);
	fscanf(fd2,"%s",bug);
	int sum=0;
	for(int i=0;i<25008;i++)
	{
		int a,b,c,d;
		fscanf(fd,"%d,%d",&c,&b);
		fscanf(fd2,"%d,%d",&d,&a);
		//printf("c=%d b=%d a=%d\n",c,b,a);
		if(a==b)
			sum++;
	}
	printf("correct %d\n",sum);
	printf("percent %lf%%\n",(double)100*sum/25008);





	return 0;
}