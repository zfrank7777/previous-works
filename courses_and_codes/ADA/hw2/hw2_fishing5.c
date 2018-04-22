#include <stdio.h>

int main(){
	int T;
	scanf("%d",&T);
	for(int x=0;x<T;x++){
		int n,k;
		scanf("%d%d",&n,&k);
		long long int optimal_fish[10002]={};
		int optimal_pond[10002]={};
		int optimal_sum=0;
		int optimal_des=1;
		long long int fish[10002]={};
		int pond[10002]={};
		for(int i=1;i<=n;i++){
			long long int F,D;
			scanf("%lld%lld",&F,&D);
			int Ftime=0;
			int current[10002]={};
			int time_left = k-i+1;
			//fill current_fish
			for(int j=0;j<time_left;j++){	
				current[Ftime]=F-Ftime*D;
				Ftime++;
				if(F-Ftime*D<=0) break;
			}
			//merge
			int c=0;
			int pre_index=0;
			int sum=0;
			long long int merging[10002]={};
			int merging_pond[10002]={};
			for(int j=0;j<time_left;j++){
				if(current[c]>fish[pre_index]){
					merging[j]=current[c];
					merging_pond[j]=i;
					c++;
				}
				else{
					merging[j]=fish[pre_index];
					merging_pond[j]=pond[pre_index];
					pre_index++;
				}
				//printf("%lld ,pre_index=%d,c=%d\n",merging[j],pre_index,c);
				sum += merging[j];
			}
			//printf("\n");
			//update fish,pond,optimal
			if(sum>optimal_sum){
				optimal_sum = sum;
				optimal_des = i;
				k = time_left;
				for(int j=0;j<k;j++){
					optimal_fish[j] = fish[j] = merging[j];
					optimal_pond[j] = pond[j] = merging_pond[j];
				}				
			}
			else
				for(int j=0;j<k;j++){
					fish[j] = merging[j];
					pond[j] = merging_pond[j];
				}
//			for(int z=0;z<10;z++)
//				printf("i=%d, optimal_fish[i]=%lld,optimal_pond[i]=%d\n", z,optimal_fish[z],optimal_pond[z]);
//			printf("opt_sum=%d\n", optimal_sum);
		}
		long long int fishsum=optimal_sum;
		int fishtime[10002]={};
		for(int i=0;i<k;i++)
			fishtime[optimal_pond[i]]++;
		int operation = 0;
		for(int i=1;i<=n;i++){
			if(fishtime[i]) operation++; // fish
			if(fishtime[i] && i!=1)	operation++; //move
		}
		printf("%lld %d\n",fishsum,operation);	
		for(int i=1;i<=optimal_des;i++){
			//printf("i=%d\n", i);
			if(fishtime[i])
				printf("fish %d\n",fishtime[i]);
			if(i!=optimal_des){
				int go=i;
				for(int j=i+1;j<=optimal_des;j++)
					if(fishtime[j]){
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