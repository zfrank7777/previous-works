/*
no heap
*/
#include <stdio.h>
int main(){
	char c;
	int S[1000009];
	S[0]=0;
	int S_index=1;
	if(scanf("%c",&c)==-1){		
		printf("0\n");
		return 0;
	}				
	int Q[1000009]={};
	int q_num=0;
	int sum=0;
	while(c!='\n'){
//		printf("%c\n", c);
		if (c=='?')	q_num++;
		Q[S_index]=q_num;
		if(c=='(')	S[S_index]=1;
		else		S[S_index]=-1;

		sum+=S[S_index];

		S_index++;
		if(scanf("%c",&c)==-1) break;
	}
	S_index--;

	long long int l,r;
	long long int LRdiff[q_num+1];
	long long int total_cost=0;
	for(int i=1;i<=q_num;i++){
		scanf("%lld%lld",&l,&r);
		LRdiff[i] = l-r;
		total_cost += r;
	}

	if(S_index%2!=0 || sum>0 || sum<-q_num){
		printf("Impossible\n");
		return 0;
	}	
	sum=0;
	for(int i=1;i<=S_index;i++){
		sum+=S[i];
		if(sum>=0)
			continue;
		long long int min=2147483646;
		int pick=-1;
		for(int j=1;j<=Q[i];j++)
			if(LRdiff[j]<min){
				min=LRdiff[j];
				pick=j;
			}
		if(pick==-1){
			printf("Impossible\n");
			return 0;
		}
		sum+=2;
		total_cost+=min;
		LRdiff[pick]=2147483647;
		//printf("pick:%d,cost diff=%d\n", pick,min);		
	}
	if(sum!=0)
		printf("Impossible\n");
	else
		printf("%lld\n", total_cost);
/*	printf("%d,%d\n", sum,S_index);
	for(int i=0;i<=S_index;i++)
		printf("%d ", S[i]);
	printf("\n");
*/
	return 0;
}