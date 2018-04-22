#include <stdio.h>

int main(){
	int T;
	scanf("%d",&T);
	for(int x=0;x<T;x++){
		int n,k;
		scanf("%d%d",&n,&k);
		long long int F[1002]={};
		long long int D[1002]={};
		int temp,temp_index;
		for(int i=1;i<=n;i++)
			scanf("%lld%lld",&F[i],&D[i]);
		int Ftime[1002]={};
		int destination=1;
		int max,pick;
		long long int fishsum=0;
		int time=0;
		while(time+destination-1<k){
			time++;
			max=0;
			pick=0;
			for(int i=1;i<=n;i++)
				if(F[i]-Ftime[i]*D[i] > max && time+i<=k){
					max=F[i]-Ftime[i]*D[i];
					pick=i;
				}
			if(pick>destination)	
				destination=pick;
			Ftime[pick]++;
			fishsum += max;
		}

		int operation=destination-1;
		for(int i=1;i<=n;i++)
			if(Ftime[i]) operation++;

		printf("%lld %d\n",fishsum,operation);	
		for(int i=1;i<=destination;i++){
			//printf("i=%d\n", i);
			if(Ftime[i])
				printf("fish %d\n",Ftime[i]);
			if(i!=destination){
				int go=i;
				for(int j=i+1;j<=destination;j++)
					if(Ftime[j]){
						go=j;
						break;
					} 
				if(go==i)	break;
				printf("move %d\n",go);
				i=go-1;
			}
		}

	}
	return 0;
}