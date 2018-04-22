#include<stdio.h>
int weight[2000001];
int w_sum[2000001]={};
int r[21]={};
int main(){
///scan input and initialize///
	int n,k;
	scanf("%d%d",&n,&k);
	for(int i=k;i>0;i--)
		r[i]=n-k+i;
	weight[0]=0;
	for(int i=1;i<=n;i++){
		scanf("%d",&weight[i]);
		for(int j=1;j<=n;j++){
			if(j>i)
				w_sum[j]+=weight[i]*(j-i);
			//printf("%d ",w_sum[j]);			
		}
		//printf("\n");
	}
	//for(int i=1;i<=k;i++)
	//	printf("%d\n",r[i]);

///start///
	int outer_flag=1;
	while(outer_flag==1){
		outer_flag=0;
		for(int i=1;i<=k-1;i++){
			int flag=1;
			while(flag==1){
				if(r[i]==r[i-1]+1)
					break;
				if( (w_sum[r[i]]) > 
					(weight[r[i]]*(r[i+1]-r[i]) + w_sum[r[i]-1]) )
				{
					//printf("i=%d,r[i]=%d,before:%d,after:%d\n",i,r[i],(w_sum[r[i]]),
					//	(weight[r[i]]*(r[i+1]-r[i]) + w_sum[r[i]-1]));
					r[i]--;
					outer_flag=1;					
				}
				else
					flag=0;
			}
		}
	}
	///calculate cost
	int cost=0;
	for(int i=1;i<=k;i++){
		for(int j=r[i-1]+1;j<=r[i];j++)
			cost+=weight[j]*(r[i]-j);
	}
	printf("%d\n",cost);
//	for(int i=1;i<=k;i++)
//		printf("%d\n",r[i]);
	return 0;
}