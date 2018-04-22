#include <stdio.h>
#include <string.h>
#include <stdlib.h>
const int N=100001;
using namespace std;
int adj_list[N][50]={{}};
int adj_from[N][50]={{}};
int outdegree[N]={};
int indegree[N]={};
int V,E;
int answer_flag=0;
bool traversed[N];
int ans[N]={};

int startpoint[N]={};
int chainlength[N]={};
int type[N]={}; //1:head,2:body.3:tail
void merge(){
	for(int i=1;i<=V;i++){
		if(type[i]>0 || outdegree[i]!=1 ) continue;
		type[i]=1;
		chainlength[i]=0;
		int nxt=i;
		while(outdegree[nxt]==1){
			chainlength[i]++;
			nxt = adj_list[nxt][0];
			if(type[nxt]==1){
				type[nxt]=2;
				startpoint[i]=startpoint[nxt];
				nxt = startpoint[nxt];
				break;			
			}
			else if(type[nxt]==2){
				printf("-1\n");
				exit(0);
			}
			else
				type[nxt]=2;
		}
		type[nxt]=3;
		startpoint[i]=nxt;
	}
	return;
}
void DFS(int now,int n){
	if(answer_flag==1) return;
	//end condition
	ans[n]=now;
	if(n>=V) return;
	if(n==V-1){
		if(now!=1) return;
		printf("1 ");
		for(int i=0;i<=n;i++)
			printf("%d%c",ans[i]," \n"[i==n]);
		answer_flag = 1;
		return; 
	}
	//traversing
	traversed[now]=true;
	if(type[now]==1){
		int next=now;
		for(int i=0;i<chainlength[now];i++){
			ans[n+i]=next;
			next = adj_list[next][0];
		}
		DFS(startpoint[now],n+chainlength[now]);
	}
	else if(type[now]==2) return;
	else
		for(int i=0;i<outdegree[now];i++)
			if(!traversed[adj_list[now][i]]) 
				DFS(adj_list[now][i],n+1);

	traversed[now]=false;
	return;
}

int main(){
	scanf("%d%d",&V,&E);
	int u,v;
	for(int i=0;i<E;i++){
		scanf("%d%d",&u,&v);
		adj_list[u][outdegree[u]]=v;
		outdegree[u]++;
		adj_from[v][indegree[v]]=u;
		indegree[v]++;
	}
	if(indegree[1]==0){
		printf("-1\n");
		return 0;
	}
	merge();
	for(int i=0;i<outdegree[1];i++)
		if(!traversed[adj_list[1][i]])
			DFS(adj_list[1][i],0);
	if(answer_flag==0)
		printf("-1\n");
	return 0;
}