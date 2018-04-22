#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>    
#include <sys/stat.h>    
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
int main(int argc, char* argv[]){
	if(argc!=2) fprintf(stderr, "input error\n");
	char rd_name[500];
	sprintf(rd_name,"host%s.FIFO",argv[1]);
	//fprintf(stderr, "befire mkfifo\n", );
	mkfifo(rd_name,0777);
	char wr_name[4][500];
	for(int i=0;i<4;i++){
		sprintf(wr_name[i],"host%s_%c.FIFO",argv[1],'A'+i);
		mkfifo(wr_name[i],0777);
	}
	int rd_FIFO=open(rd_name,O_RDWR);
	int wr_FIFO[4];
	for(int i=0;i<4;i++)
		wr_FIFO[i] = open(wr_name[i],O_RDWR);
//	fprintf(stderr,"%d %d %d %d %d\n",rd_FIFO,wr_FIFO[0],wr_FIFO[1],wr_FIFO[2],wr_FIFO[3]);
	int player_id[4];
	char player_id_str[5][2];
	strcpy(player_id_str[0],"A");
	strcpy(player_id_str[1],"B");
	strcpy(player_id_str[2],"C");
	strcpy(player_id_str[3],"D");
	while(1){
	//	fprintf(stderr, "entering while loop\n" );
		char id_temp[1200];
		//memset(id_temp,0,sizeof(id_temp));
		scanf("%d%d%d%d",&player_id[0],&player_id[1],&player_id[2],&player_id[3]);
		//read(STDIN_FILENO,id_temp,1024);
		//sscanf(id_temp,"%d%d%d%d",&player_id[0],&player_id[1],&player_id[2],&player_id[3]);
//		fprintf(stderr,"%d %d %d %d\n",player_id[0],player_id[1],player_id[2],player_id[3]);
		int exit_flag=1;
		for(int i=0;i<4;i++)
			if(player_id[i]!=-1) exit_flag=0;
		if(exit_flag){
			close(rd_FIFO);
			unlink(rd_name);
			for(int i=0;i<4;i++){
				close(wr_FIFO[i]);
				unlink(wr_name[i]);
			}
			return 0;
		} 

		int money[4]={1000,1000,1000,1000};
		pid_t pid[4];
		int key[4]={0,1,2,3};
		char key_str[5][100];
		strcpy(key_str[0],"0");
		strcpy(key_str[1],"1");
		strcpy(key_str[2],"2");
		strcpy(key_str[3],"3");
		for(int i=0;i<4;i++){
			pid[i]=fork();
		//	fprintf(stderr,"i=%d, pid=%d\n",i,pid[i]);
			if((pid[i])<0) fprintf(stderr,"fork error\n");
			else if(pid[i]==0){
		//		fprintf(stderr,"child ,%s,%s\n",player_id_str[i],key_str[i]);
				if(execlp("./player","./player",argv[1],player_id_str[i],key_str[i],(char *)0)<0)
					fprintf(stderr,"execl error\n");
			}
		}
		char wr_temp[1200];
		char rd_temp[1200];
		int win_time[4]={};
		for(int i=0;i<10;i++){
	//		fprintf(stderr, "i=%d\n",i);
		//writing to FIFO
		//	fprintf(stderr, "write\n" );
			memset(wr_temp,0,sizeof(wr_temp));
	//		fprintf(stderr,"money:%d,%d,%d,%d\n",money[0],money[1],money[2],money[3]);
			sprintf(wr_temp,"%d %d %d %d\n",money[0],money[1],money[2],money[3]);
			for(int j=0;j<4;j++)
				write(wr_FIFO[j],wr_temp,strlen(wr_temp));
		//reading bids
			int bid[4];
			int read_flag=0;
			while(read_flag<4){
	//			fprintf(stderr, "read, j=%d, rd_FIFO=%d\n",j,rd_FIFO );
				memset(rd_temp,0,sizeof(rd_temp));
				if(read(rd_FIFO,rd_temp,1024)<0) fprintf(stderr, "read error\n");
			//	fprintf(stderr, "after read: rd_buffer= %s\n",rd_temp );
				int num_of_input=0;
				int cut[5]={};
				for(int k=0;k<strlen(rd_temp);k++)
					if(rd_temp[k]=='\n'){
						num_of_input++;
						cut[num_of_input]=k+1;
					}
				read_flag+=num_of_input;
				for(int k=0;k<num_of_input;k++){
					char player_index;
					int player_key,player_bid;
					sscanf(&rd_temp[cut[k]],"%c %d %d",&player_index,&player_key,&player_bid);
					player_index -= 'A';
					if(key[player_index]!=player_key) fprintf(stderr, "key error:key[idx]=%d,key=%d\n",key[player_index],player_key);
					if(player_bid > money[player_index]) fprintf(stderr, "bid error:bid=%d,money[idx]\n",player_bid,money[player_index]);
					bid[player_index] = player_bid;
			//		fprintf(stderr,"record:player_idx=%d, bid=%d\n",player_index,player_bid);
				}
			}
		// jugding results
		//	fprintf(stderr,"bid: %d, %d, %d, %d\n",bid[0],bid[1],bid[2],bid[3]);
		//	fprintf(stderr, "judge\n" );
			int max_bid,max_bidder,second_bid,second_bidder;
			max_bid = second_bid = max_bidder = second_bidder = -1;
			for(int j=0;j<4;j++){
				if(bid[j] > max_bid){
					second_bidder = max_bidder;
					second_bid = max_bid;
					max_bidder = j;
					max_bid = bid[j];
				}
				else if(bid[j]==max_bid){
					max_bidder = second_bidder;
					max_bid = second_bid;
					second_bid = second_bidder = -1;
				}
				else if(bid[j] > second_bid){
					second_bidder = j;
					second_bid = bid[j];
				}
			}
		//recording result	
//			fprintf(stderr, "record:winner=%d\n",max_bidder );
			int alpha[4]={};
			if(max_bidder>=0) {
				alpha[max_bidder]=1;
				win_time[max_bidder]++;
			}
			for(int j=0;j<4;j++){
			//	fprintf(stderr,"i=%d, j=%d\nalpha[j]=%d\n",i,j,alpha[j]);
			//	fprintf(stderr,"before:money[j]=%d\n",money[j]);
				money[j] = 1000 + money[j] - alpha[j]*bid[j];
			//	fprintf(stderr,"after:money[j]=%d\n",money[j]);
			}
		}
//		fprintf(stderr, "ranking\n" );
		int rank[4]={};
		for(int i=1;i<=4;i++){
			int max=0;
			int num=-1;
			for(int j=0;j<4;j++)
				if(win_time[j] > max)
					max = win_time[j];
			for(int j=0;j<4;j++)
				if(win_time[j]==max){
					rank[j]=i;
					win_time[j]=-1;
					num++;
				}
			i += num;
		}
		printf("%d %d\n%d %d\n%d %d\n%d %d\n",player_id[0],rank[0],player_id[1],rank[1],player_id[2],rank[2],player_id[3],rank[3]);
		fflush(stdout);

		for(int i=0;i<4;i++)
			waitpid(pid[i],NULL,0);
	//	fprintf(stderr, "end\n" );
	}
	fprintf(stderr,"%s\n",rd_name);
	close(rd_FIFO);
	unlink(rd_name);
	for(int i=0;i<4;i++){
		close(wr_FIFO[i]);
		fprintf(stderr,"%s\n",wr_name[i]);
	}
	return 0;
}
