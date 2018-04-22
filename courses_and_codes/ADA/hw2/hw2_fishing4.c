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
			long long int MB[10002]={};		// fish gained from fishing n times
			long long int now[10002]={};	// F,F-D,F-2D,F-3D...(indexed from 0)
			int Ftime=0;		// time you fish this pond
			int movecost=0;			// cost = movecost + fishcost
			short flag=0;
			int last=k-1;
			for(int y=0;y<i-destination;y++){
				movecost += prefish[last];		//movecost
				last--;
			}
			while(F-Ftime*D>0 && Ftime<k-i+destination){
				now[Ftime] = F-Ftime*D;
				MB[Ftime] = now[Ftime] - prefish[last] ;			// MB = gain - fishcost
				last--;
				if(Ftime) MB[Ftime]+=MB[Ftime-1];
				//printf("MB=%lld\n", MB[Ftime]);
				if(MB[Ftime]>movecost && flag==0){
					flag=1;
					//k-=i-destination;
					destination=i;
				} 
				if(Ftime)
					if(MB[Ftime]<movecost && MB[Ftime-1]>movecost) break;
				Ftime++;
			}

			if(flag){
				int t=0;
				int f=1;
				for(int j=0;j<k;j++){
					if(F-t*D > prefish[j] && f){
						nowfish[j] = F-t*D;					
						pond[j]=i;
						if(t==Ftime) f=0;
						t++;
					}
					else{
						nowfish[j] = prefish[j-t];
						pond[j] = prepond[j-t];
					}					
				}
				for(int j=0;j<k;j++){
					prefish[j]=nowfish[j];
					prepond[j]=pond[j];
				}

			}

/*			printf("i=%d,k=%d,flag=%d,Ftime=%d\n", i,k,flag,Ftime);
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