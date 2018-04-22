#include <stdio.h>
int F[1002][10002]={{}};
int main(){
	int T;
	scanf("%d",&T);
	for(int x=0;x<T;x++){
		int n,k;
		scanf("%d%d",&n,&k);
		for(int i=0;i<1002;i++)
			for(int j=0;j<10002;j++)
				F[i][j]=0;
		int D[1002]={};
		int temp,temp_index;
		for(int i=1;i<=n;i++){
			scanf("%d%d",&F[i][0],&D[i]);
			temp=F[i][0];
			temp_index=1;
			while(temp>0){
				temp-=D[i];
				if(temp<0) temp=0;
				F[i][temp_index]=temp;
				temp_index++;
				if(temp_index>k)
					break;
			}
		}
		int Ftime[1002]={};
		int destination=1;
		int now=1;
		int movetime=0;
		int max,pick;
		long long int fishsum=0;
		int time=0;
		while(time+movetime<k){
			time++;
			max=0;
			pick=0;
			for(int i=1;i<=n;i++)
				if(F[i][Ftime[i]]>max){
					if(i>destination)	destination=i;
					max=F[i][Ftime[i]];
					pick=i;
				}
			if(Ftime[pick]==0 && pick!=1 && pick!=0){
				movetime++;
				//printf("pick:%d\n", pick);
			}	
			Ftime[pick]++;
			fishsum += max;
		}

/*		printf("F:\n");
		for(int i=1;i<=n;i++){
			for(int j=0;j<=10;j++)
				printf("%3d ", F[i][j]);
			printf("\n");
		}
		printf("Ftime:\n");
		for(int i=1;i<=n;i++)
			printf("%3d ", Ftime[i]);
		printf("\n");
*/		
		int operation=movetime;
		for(int i=1;i<=n;i++)
			if(Ftime[i]) operation++;
	
		printf("%lld %d\n",fishsum,operation);	
		for(int i=1;i<=n;i++){
			if(Ftime[i])
				printf("fish %d\n",Ftime[i]);
			if(i!=destination){
				int j;
				for(j=i+1;j<=destination;j++)
					if(Ftime[j]) break;
				printf("move %d\n",j);
			}
			else
				break;
		}

	}
	return 0;
}