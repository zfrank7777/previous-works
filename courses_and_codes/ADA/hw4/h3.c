#include <stdio.h>
#include <string.h>
const int N=100001;
using namespace std;
int adj_list[N][50]={{}};
int outdegree[N]={};
int indegree[N]={};
int V,E;
int answer_flag=0;
bool traversed[N];
int n=1;
int ans[N]={};

int startpoint[N]={};
int chainlength[N]={};
//int endpoint[N]={};

void merge(){
	bool skipped[N]={};
	for(int i=1;i<=V;i++){
		if(skipped[i]==true || outdegree[i]!=1 )
			continue;
		else{
			chainlength[i]=1;
			int nxt=adj_list[i][0];
			skipped[nxt]=true;
			while(outdegree[nxt]==1 && indegree[nxt]==1){
				chainlength[i]++;
				nxt = adj_list[nxt][0];
				skipped[nxt]=true;
			}
			startpoint[i]=nxt;
			//endpoint[nxt]=i;
		}
	}
	return;
}

void DFS(int now){
	//end condition
	if(n==V){
		for(int i=0;i<outdegree[now];i++)
			if(adj_list[now][i]==1){
				answer_flag=1;
				ans[n]=now;
				return;
			}
		return;
	}
	//traversing
	if(startpoint[now]!=0){
		n+=chainlength[now];
		DFS(startpoint[now]);
		n-=chainlength[now];
		int tmp=now;
		if(answer_flag==1){
			for(int i=0;i<chainlength[now];i++){
				ans[n+i]=tmp;
				tmp = adj_list[tmp][0];
			}
		}
		return;
	}
	else
		for(int i=0;i<outdegree[now];i++){
			if(traversed[adj_list[now][i]])
				continue;
			n++;
			traversed[i]=true;
			//printf("now:%d goto %d\n",now,adj_list[now][i] );
			DFS(adj_list[now][i]);
			//printf("(%d,%d) return\n",now,adj_list[now][i] );
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
		indegree[v]++;
	}
	traversed[1]=true;
	if(indegree[1]==0){
		printf("-1\n");
		return 0;
	}
	merge();
	//for(int i=1;i<=V;i++)
	//	printf("%d,%d,%d\n",i,startpoint[i],chainlength[i] );
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