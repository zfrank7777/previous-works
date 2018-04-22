#include <stdio.h>
#include <string.h>
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
	traversed[now]=true;
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
	for(int i=0;i<outdegree[1];i++)
		if(!traversed[adj_list[1][i]])
			DFS(adj_list[1][i],0);
	if(answer_flag==0)
		printf("-1\n");
	return 0;
}