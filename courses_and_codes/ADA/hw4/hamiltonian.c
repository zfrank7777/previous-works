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
int n=1;
int ans[N]={};
void DFS(int now){
	//end condition
	if(n==V){
		for(int i=0;i<indegree[1];i++)
			if(adj_from[1][i]==now){
				answer_flag=1;
				ans[n]=now;
				return;
			}
		return;
	}
	//traversing
	for(int i=0;i<outdegree[now];i++){
		if(traversed[adj_list[now][i]])
			continue;
		n++;
		traversed[i]=true;
		DFS(adj_list[now][i]);
		if(answer_flag==1){
			n--;
			ans[n]=now;
			return;
		}
		n--;
		traversed[i]=false;
	}
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
	traversed[1]=true;
	if(indegree[1]==0){
		printf("-1\n");
		return 0;
	}
	DFS(1);
	if(answer_flag==0)
		printf("-1\n");
	else{
		for(int i=1;i<=V;i++)
			printf("%d ", ans[i]);
		printf("1\n");
	}

	return 0;
}