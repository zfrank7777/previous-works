#include <stdio.h>

int main(){
	int T;
	scanf("%d",&T);
	for(int x=0;x<T;x++){
		int n,k;
		scanf("%d%d",&n,&k);
		long long int prefish[10002]={}; // record each fish number in the previous pond
		int prepond[10002]={};
		int pond[10002]={};
		int destination=1;
		for(int i=1;i<=n;i++){
			long long int F,D;
			scanf("%lld%lld",&F,&D);
			long long int nowfish[10002]={};
			int Ftime=0;				// time you fish this pond
			for(int j=0;j<k;j++){
				if(F-Ftime*D > prefish[j]){
					if(Ftime==0){
						int movecost=0;
						for(int y=k-i+destination;y<k;y++)
							movecost += prefish[y];
						if(F<movecost){
							nowfish[j] = prefish[j-Ftime];
							pond[j] = prepond[j-Ftime];									
							continue;
						} 
					}
					if(Ftime==0 && i!=1){
						k-=i-destination;
						destination=i;
					} 
					nowfish[j] = F-Ftime*D;					
					Ftime++;
					pond[j]=i;
				}
				else{
					nowfish[j] = prefish[j-Ftime];
					pond[j] = prepond[j-Ftime];
				}
			}
			for(int j=0;j<k;j++){
				prefish[j]=nowfish[j];
				prepond[j]=pond[j];
			}
/*			printf("i=%d,k=%d\n", i,k);
			for(int w=0;w<k;w++)
				printf("%lld,%d\n", prefish[w],prepond[w]);
			printf("\n");
*/
		}
		long long int fishsum=0;
		int fishtime[1002]={};
		for(int i=0;i<k;i++){
			fishsum += prefish[i];
			fishtime[prepond[i]]++;
		}
		int operation = 0;
		for(int i=1;i<=n;i++){
			if(fishtime[i]) operation++; // fish
			if(fishtime[i] && i!=1)	operation++; //move
		}

		printf("%lld %d\n",fishsum,operation);	
//		for(int i=0;i<k;i++)
//			printf("%lld,%d\n", prefish[i],prepond[i]);
//		printf("\n");
		for(int i=1;i<=destination;i++){
			//printf("i=%d\n", i);
			if(fishtime[i])
				printf("fish %d\n",fishtime[i]);
			if(i!=destination){
				int go=i;
				for(int j=i+1;j<=destination;j++)
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