#include<stdio.h>
int main(){
	int n,p;
	scanf("%d%d",&n,&p);
	int cointype[10010];
	int max[10010]={0};
	max[0]=1;
	for(int i=0;i<n;i++){
		scanf("%d",&cointype[i]);
		for(int j=cointype[i];j<=p;j++){
			max[j]+=max[j-cointype[i]];
			max[j]%=1000000007;
		}
	}
	printf("%d\n",max[p]);
	return 0;
}
//reference:演算法筆記knapsack problem
