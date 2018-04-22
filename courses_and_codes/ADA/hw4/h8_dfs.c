#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
const int N=100001;
using namespace std;
int indegree[N]={};
int V,E;
int answer_flag=0;
bool traversed[N];
int ans[N]={};
int path[N]={};

vector<int> adj_list[N];
vector<int> chain[N]; 
bool visit[N]={};
void merge(int curr){
	if(adj_list[curr].size()==0) return;
	int nxt = adj_list[curr][0];
	visit[nxt]=true;
	if(nxt!=1 && curr!=1 && adj_list[curr].size()==1 && indegree[nxt]==1){
		//printf("a\n");
		//if(visit[adj_list[curr][0]]) return;
		adj_list[curr]=adj_list[nxt];
		traversed[nxt]=true;
		chain[curr].push_back(nxt);
		merge(curr);
	}
	else {
		for(int i=1;i<adj_list[curr].size();i++)
			if(!visit[adj_list[curr][i]])
				merge(adj_list[curr][i]);

	}
	return;
}
void DFS(int now,int path_n,int total_n){
	if(answer_flag==1) return;
	path[path_n]=now;
	path_n++;
	total_n+= (1+chain[now].size());
	if(total_n==V){
//		if(now!=1) return;
		ans[0]=1;
		int idx=1;
//		for(int i=0;i<path_n;i++)
//			printf("%d%c",path[i]," \n"[i==path_n-1]);
		for(int i=0;i<path_n;i++){
			ans[idx++]=path[i];
			for(int j=0;j<chain[path[i]].size();j++)
				ans[idx++]=chain[path[i]][j];
		}
		for(int i=0;i<idx;i++)
			printf("%d%c",ans[i]," \n"[i==idx-1]);

		answer_flag = 1;
		return; 
	}
	//traversing
	if(now==1) return;
	traversed[now]=true;
	for(int i=0;i<adj_list[now].size();i++)
		if(!traversed[adj_list[now][i]]) 
			DFS(adj_list[now][i],path_n,total_n);
	traversed[now]=false;
	return;
}

int main(){
	scanf("%d%d",&V,&E);
	int u,v;
	for(int i=0;i<E;i++){
		scanf("%d%d",&u,&v);
		adj_list[u].push_back(v);
		indegree[v]++;
	}	
	visit[1]=true;
	merge(1);
//	for(int i=1;i<=V;i++)	
//		printf("%d%c",traversed[i]," \n"[i==V]);
	for(int i=0;i<adj_list[1].size();i++)
		if(!traversed[adj_list[1][i]])
			DFS(adj_list[1][i],0,0);
	if(answer_flag==0)
		printf("-1\n");
	return 0;
}