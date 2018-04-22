#include<stdio.h>

//calculate sum[j][i]
int cost_saved(int target,int mark[2001],){
	if(mark[target])
		return 0;
	//找左右的r
	int left,right;
	while()
}

int main(){
	int n,k;
	int weight[2001];
	int w_sum[2001];
	int sum[21][2001]={{}};
	scanf("%d%d",&n,&k);
	for(int i=1;i<=n;i++){
		scanf("%d",&weight[i]);
		sum[0][i]=sum[0][i-1]+weight[i];	
	}
	int max;
	int r[21];
	int r_index=0;
	int mark[2001]={};
	max=0;
	for(int j=1;j<=k;j++){
		r_index++;
		printf("r_index=%d,\n",r_index);	
		for(int i=1;i<=n;i++){
			sum[j][i]=sum[j-1][i]*(n-i);
			if(sum[j][i]>max){
				max=sum[j][i];
				r[r_index]=i;
			}
			printf("%d ",sum[j][i]);
		}
		mark[r_index]=1;
		printf("\n");			
	}	

	return 0;
}