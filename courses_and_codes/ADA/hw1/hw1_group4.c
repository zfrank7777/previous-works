#include<stdio.h>
int main(){
	int n,k,w;
	unsigned long long int sum[22][2004]={{0}};
	unsigned long long int total_cost,total_saved;
	total_cost=total_saved=0;
	scanf("%d%d",&n,&k);
	for(int i=1;i<=n;i++){
		scanf("%d",&w);
		total_cost+=w*(n-i);		//for final answer: total_cost - total_saved
		sum[1][i]=sum[1][i-1]+w;	//set up:sum of previous
		sum[2][i]=sum[1][i]*(n-i);	//k=2
	}		
	for(int j=3;j<=k;j++){
		for(int i=1;i<=n;i++){
			//calculate sum[j][i]
			unsigned long long int t=0;
			for(int x=i+1;x<=n-j+2;x++){
				t = sum[j-1][x]+sum[1][i]*(x-i);//sum[j][i]=max(last row)*distance+last column last row
				if(t>sum[j][i])  sum[j][i]=t;
			}
		}
	}	
	for(int i=1;i<=k;i++){
		for(int j=1;j<=n;j++)
			printf("%4llu ",sum[i][j] );
		printf("\n");
	}
	for(int i=1;i<=n;i++)
		if(sum[k][i]>total_saved)
			total_saved=sum[k][i];
	if(k==n)
		printf("0");	
	else if(k==1)
		printf("%llu",total_cost);
	else
		printf("%llu",total_cost-total_saved);
	return 0;
}