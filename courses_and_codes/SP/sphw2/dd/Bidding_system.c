#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>    
#include <sys/stat.h>    
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>

int main(int argc, char* argv[]){
	if(argc!=3) fprintf(stderr, "input error\n");
	int host_num,player_num;
	sscanf(argv[1],"%d",&host_num);
	sscanf(argv[2],"%d",&player_num);
//	printf("%d %d\n",host_num,player_num);
	char FIFO_name[100][2][1200];
	//char name_temp[1200];
	int FIFO[100][2]; 	// [0]:sys to host, [1]:host to sys
	for(int i=0;i<host_num;i++){
		memset(FIFO_name[i][0],0,sizeof(FIFO_name[i][0]));
		sprintf(FIFO_name[i][0],"s_to_h%d.FIFO",i);
		mkfifo(FIFO_name[i][0],0666);
		FIFO[i][0] = open(FIFO_name[i][0],O_RDWR);
		memset(FIFO_name[i][1],0,sizeof(FIFO_name[i][1]));
		sprintf(FIFO_name[i][1],"h_to_s%d.FIFO",i);
		mkfifo(FIFO_name[i][1],0666);		
		FIFO[i][1] = open(FIFO_name[i][1],O_RDWR);
		
	//	fprintf(stderr,"i=%d, fifo = %d, %d\n",i,FIFO[i][0],FIFO[i][1]);
	}

	int pid[host_num];
	for(int i=0;i<host_num;i++){
		if((pid[i]=fork())<0)
			fprintf(stderr, "fork error\n");
		else if(pid[i]==0){
			//close(0);
			dup2(FIFO[i][0],0);
			//close(1);
			dup2(FIFO[i][1],1);
			char host_id[100];
			sprintf(host_id,"%d",i+1);
			if(execlp("./host","./host",host_id,(char *)0)<0) fprintf(stderr, "exec error\n");
			fprintf(stderr,"exec error\n");
		}
	}
//	select variables
  	int selectreturn;
	fd_set fd_temp;
   	struct timeval timeout;
   	timeout.tv_sec = 1;
    	timeout.tv_usec = 0;	
    	int maxfd = getdtablesize();

	int score[10000]={};
	int player_temp[4];
	int rank_temp[4];
	int first=0;
	int host;
	char wr_buff[1200];
	char rd_buff[1200];
	for(int a=0;a<player_num;a++)
		for(int b=a+1;b<player_num;b++)
			for(int c=b+1;c<player_num;c++)
				for(int d=c+1;d<player_num;d++){
					host=-1;
				//	fprintf(stderr,"first=%d\n",first);
					//first (num_of_host) competition
					if(first<host_num){
						host = first;
						first++;
					}
				//select
					else{
						while(1){
				//			fprintf(stderr,"%d\n",first);
						//select
							FD_ZERO(&fd_temp);
							for(int i=0;i<host_num;i++)
								FD_SET(FIFO[i][1],&fd_temp);
							selectreturn = select(maxfd,&fd_temp,NULL,NULL,&timeout);
							if(selectreturn==0)
								continue;
							host=-1;
							for(int i=0;i<maxfd;i++)
								if(FD_ISSET(i,&fd_temp)){
									host=i/2-2;
									break;
								}
						//	fprintf(stderr,"host=%d\n",host);
						//read
							if(host<0) fprintf(stderr, "host error\n");
							memset(rd_buff,0,sizeof(rd_buff));
							if(read(FIFO[host][1],rd_buff,1024)<0) fprintf(stderr, "read error\n" );
							int read_playe_num=0;
							for(int i=0;i<strlen(rd_buff);i++)
								if(rd_buff[i]=='\n') read_playe_num++;
							if(read_playe_num!=4) fprintf(stderr, "read content error:\n%s\n",rd_buff);
							sscanf(rd_buff,"%d%d%d%d%d%d%d%d",
									&player_temp[0],&rank_temp[0],&player_temp[1],&rank_temp[1],
									&player_temp[2],&rank_temp[2],&player_temp[3],&rank_temp[3]);
							for(int i=0;i<4;i++)
								score[player_temp[i]] += 4 - rank_temp[i];
							break;		
						}
					}
				// write to FIFO
					if(host<0) fprintf(stderr, "host error\n");
					memset(wr_buff,0,sizeof(wr_buff));
					sprintf(wr_buff,"%d %d %d %d\n",a,b,c,d);
					if(write(FIFO[host][0],wr_buff,strlen(wr_buff))<0) fprintf(stderr, "write error\n");
				//	fprintf(stderr,"wr_buff: %s\n",wr_buff);
					
				}
// read all results
	while(first>0){
	//select
		FD_ZERO(&fd_temp);
		for(int i=0;i<host_num;i++)
			FD_SET(FIFO[i][1],&fd_temp);
		selectreturn = select(maxfd,&fd_temp,NULL,NULL,&timeout);
		if(selectreturn==0)
			continue;
		first--;
		host=-1;
		for(int i=0;i<maxfd;i++)
			if(FD_ISSET(i,&fd_temp)){
				host=i/2-2;
				break;
			}
//		fprintf(stderr,"host=%d\n",host);
	//read
		if(host<0) fprintf(stderr, "host error\n");
		memset(rd_buff,0,sizeof(rd_buff));
//		fprintf(stderr,"before read\n");
		if(read(FIFO[host][1],rd_buff,1024)<0) fprintf(stderr, "read error\n" );
//		fprintf(stderr,"read:rd_buff=%s\n",rd_buff);
		int read_playe_num=0;
		for(int i=0;i<strlen(rd_buff);i++)
			if(rd_buff[i]=='\n') read_playe_num++;
		if(read_playe_num!=4) fprintf(stderr, "read content error:\n%s\n",rd_buff);
		sscanf(rd_buff,"%d%d%d%d%d%d%d%d",
				&player_temp[0],&rank_temp[0],&player_temp[1],&rank_temp[1],
				&player_temp[2],&rank_temp[2],&player_temp[3],&rank_temp[3]);
		for(int i=0;i<4;i++)
			score[player_temp[i]] += 4 - rank_temp[i];		
	}
//	terminate all hosts
	memset(wr_buff,0,sizeof(wr_buff));
	sprintf(wr_buff,"-1 -1 -1 -1\n");
	for(int i=0;i<host_num;i++)
		write(FIFO[i][0],wr_buff,strlen(wr_buff));
//	fprintf(stderr, "ranking\n" );
//	for(int i=0;i<player_num;i++)
//		fprintf(stderr,"i=%d, score=%d\n",i,score[i]);
	int rank[10000]={};
	for(int i=0;i<player_num;i++){
		rank[i] = 1;
		for(int j=0;j<player_num;j++)
			if(score[j] > score[i])
				rank[i]++;
		printf("%d %d\n",i+1,rank[i]);
	}
	
	for(int i=0;i<host_num;i++){
		close(FIFO[i][0]);
		close(FIFO[i][1]);
//		fprintf(stderr,"%s %s\n",FIFO_name[i][0],FIFO_name[i][1]);
		unlink(FIFO_name[i][0]);
		unlink(FIFO_name[i][1]);
	}

	return 0;
}
