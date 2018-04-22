#include <stdio.h>
#include <queue>
#include <iostream>
using namespace std;
int main(){
	char c;
	int S[1000009]={};
	S[0]=0;
	int S_index=1;
	if(scanf("%c",&c)==-1){		
		printf("0\n");
		return 0;
	}				
	int Q[1000009]={};
	int q_num=0;
	while(c!='\n'){
		if (c=='?'){
			q_num++;
			S[S_index]=0;
		}	
		else if(c=='(')	S[S_index]=1;
		else			S[S_index]=-1;
		Q[S_index]=q_num;
		S_index++;
		if(scanf("%c",&c)==-1) break;
	}
	S_index--;
    priority_queue<long long int, vector<long long int>, greater<long long int> > pq;	
    long long int l,r;
	long long int LRdiff[q_num+1];
	long long int total_cost=0;
	for(int i=1;i<=q_num;i++){
		scanf("%lld%lld",&l,&r);
		LRdiff[i] = l-r;
		total_cost += r;
	}
	if(S_index%2!=0){
		printf("Impossible\n");
		return 0;
	}	
	int sum=0;
	for(int i=1;i<=S_index;i++){
		if(S[i]==0){
			S[i]=-1;
			pq.push(LRdiff[Q[i]]);
		}
		sum+=S[i];
		if(sum>=0)
			continue;
		if(pq.empty()){
			printf("Impossible\n");
			return 0;
		}
		sum+=2;
		total_cost += pq.top();
		pq.pop();
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