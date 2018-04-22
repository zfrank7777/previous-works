#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>    
#include <sys/stat.h>    
#include <fcntl.h>
#include <string.h>
#include <time.h>
int main(int argc,char* argv[]){
	if(argc!=4) fprintf(stderr,"input error\n");
	char* host_id = argv[1];
	int player_index = *argv[2]-'A';
	char* random_key = argv[3];
	int player_money[4]={};
	char write_name[500];
	char read_name[500];
	sprintf(read_name,"host%s_%s.FIFO",argv[1],argv[2]);
	sprintf(write_name,"host%s.FIFO",argv[1]);
	int read_FIFO,write_FIFO;
	write_FIFO = open(write_name,O_RDWR);
	read_FIFO = open(read_name,O_RDWR);
	for(int i=0;i<10;i++){
		char rd_temp[1200];
		int money[4];
		memset(rd_temp,0,sizeof(rd_temp));
		if(read(read_FIFO,rd_temp,1024)<0) fprintf(stderr,"read error\n");
		sscanf(rd_temp,"%d %d %d %d",&money[0],&money[1],&money[2],&money[3]);
		char wr_temp[1200];
		//bonus strategy: 3/4 of my money + random();
		srand(time(NULL));
		int bid = player_money[player_index]*3/4 + (rand()%10);
		sprintf(wr_temp,"%s %s %d\n",argv[2],argv[3],bid);
		write(write_FIFO,wr_temp,strlen(wr_temp));
	}
	close(write_FIFO);
	close(read_FIFO);
	return 0;
}
