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
bool skipped[N]={};
void merge(){
	for(int i=1;i<=V;i++){
		if(skipped[i]==true || outdegree[i]!=1 )
			continue;
		else{
			chainlength[i]=1;
			int nxt=adj_list[i][0];
			if(startpoint[nxt]!=0)
			skipped[nxt]=true;
			while(outdegree[nxt]==1 && indegree[nxt]==1){
				chainlength[i]++;
				nxt = adj_list[nxt][0];
				skipped[nxt]=true;
			}
			startpoint[i]=nxt;
		}
	}
	return;
}

void DFS(int now,int n){
	if(answer_flag==1) return;
	//end condition
	ans[n]=now;
	if(n==V-1){
		if(now!=1) return;
		printf("1 ");
		for(int i=0;i<=n;i++)
			printf("%d%c",ans[i]," \n"[i==n]);
		answer_flag = 1;
		return; 
	}
	//traversing
	if(skipped[now]) return;
	traversed[now]=true;
	if(startpoint[now]!=0){
		int next=now;
		for(int i=0;i<chainlength[now];i++){
			ans[n+i]=next;
			next = adj_list[next][0];
		}
		DFS(startpoint[now],n+chainlength[now]);
	}
	else
		for(int i=0;i<outdegree[now];i++){
			if(traversed[adj_list[now][i]])
				continue;
			DFS(adj_list[now][i],n+1);
		}
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