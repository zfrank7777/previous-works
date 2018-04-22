#include <stdio.h>
#include <string.h>
#include <vector>
#include <queue>
using namespace std;

typedef struct Node
{
	int d;
	int id;
}node;
bool operator<(node u1,node u2) { return u1.d>u2.d; }
vector<int> adj_list[200002];
priority_queue<node> degree_queue;
int degree[200002]={};
void qpush(int n,int d){
	node newnode;
	newnode.d=d;
	newnode.id=n;
	degree_queue.push(newnode);
	return;
}
int done[200002]={};
int main(){
	int V,E;
	scanf("%d%d",&V,&E);
	if(E==0){
		printf("1\n");
		return 0;
	}
	int v1,v2;
	for(int i=0;i<E;i++){
		scanf("%d%d",&v1,&v2);
		adj_list[v1].push_back(v2);
		adj_list[v2].push_back(v1);
		degree[v1]++;
		degree[v2]++;
	}
	for(int i=0;i<V;i++)
		qpush(i,degree[i]);
	int maxans=2;
	for(int i=0;i<V;i++){
		node v = degree_queue.top();
		degree_queue.pop();
		done[v.id]=1;
		int fourflag=1;
		//printf("\ni=%d, v.id=%d, v.d=%d, degree[vid]=%d \n",i,v.id,v.d,degree[v.id]);
		if(degree[v.id]<=0) continue;
		if(degree[v.id]<3) fourflag=0;
		for(int j=0;j<2;j++){
			int u = adj_list[v.id][j];
			if(done[u]>0){
				adj_list[v.id].erase(adj_list[v.id].begin()+j);
				v.d--;
				j--;
				continue;
			}
			//printf("not skipped:v.id=%d, u=%d\n",v.id,u );
			int adj[200002]={};
			for(int k=0;k<adj_list[u].size();k++){
				//printf("adj add %d\n",adj_list[u][k] );
				adj[adj_list[u][k]]++;
			}
			for(int k=j+1;k<v.d;k++){
				//printf("k=%d\n", k);
				if(done[adj_list[v.id][k]]>0)
					continue;
				if(adj[adj_list[v.id][k]]>0)
					maxans=3;
				else
					fourflag=0;
			}
			//printf("ans=%d\n",maxans );
		}
		if(fourflag){
			printf("4\n");
			return 0;
		}
		for(int j=0;j<v.d;j++){
			degree[adj_list[v.id][j]]--;
		}
	}
	printf("%d\n", maxans);
	return 0;
}