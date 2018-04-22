#include<stdio.h>
int main(){
	int n,k;
	int weight[2001];
	int sum[21][2001]={{}};
	int mark[2001]={};
	int total_cost,total_saved,r_index;
	total_cost=total_saved=r_index=0;
	scanf("%d%d",&n,&k);	
	for(int i=1;i<=n;i++){
		scanf("%d",&weight[i]);
		total_cost+=weight[i]*(n-i);
	}
//	printf("n=%d,k=%d,total_cost=%d\n",n,k,total_cost);
	for(int j=1;j<=k-1;j++){
		int max_cost_saved=0;
		for(int i=1;i<=n;i++){
			//calculate sum[j][i]
			if(mark[i])
				continue;				
			int left,right;
			left=right=i;
			while(mark[left]==0){
				left--;
				if(left==0)
					break;
			}
			while(mark[right]==0){
				if(right==n)
					break;
				right++;
			}			
			//calculate cost saved 
			for(int k=left+1;k<=i;k++)
				sum[j][i]+=weight[k]*(right-i);
			if(sum[j][i]>max_cost_saved){
				max_cost_saved=sum[j][i];
				r_index=i;
			}
//			printf("%d,%d,%d\n",left,right,total_saved);
		}
		mark[r_index]=1;
		total_saved+=max_cost_saved;
	}	
	printf("%d\n",(total_cost - total_saved));
	return 0;
}