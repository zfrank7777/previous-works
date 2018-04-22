#include <stdio.h>
#include <string.h>
#include <vector>
#include <queue>
#include <set>
using namespace std;
///
typedef struct Node
{
	int d;
	int id;
	set<int> neighbor;
}node;
node graph[200002];

bool operator<(node u1,node u2) { return u1.d>u2.d; }
priority_queue<node> degree_queue;

set<int> adj_list[200002];
int degree[200002]={};
int done[200002]={};
void record(int n,int d){
	graph[n].d=d;
	graph[n].id=n;
	return;
}

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
		graph[v1].neighbor.insert(v2);
		graph[v2].neighbor.insert(v1);
		degree[v1]++;
		degree[v2]++;
	}
	for(int i=0;i<V;i++)
		record(i,degree[i]);
	int maxans=2;
	for(int i=0;i<V;i++){
		if(degree[i]<4 && done[i]==0)
			degree_queue.push(graph[i]);
		while(!degree_queue.empty()){
			node v = degree_queue.top();
			degree_queue.pop();
			done[v.id]=1;
			//printf("\ni=%d, v.id=%d, v.d=%d, degree[vid]=%d \n",i,v.id,v.d,degree[v.id]);
			if(degree[v.id]<=0) continue;
			
			if(degree[v.id]<3 && maxans==3);
			else if(degree[v.id]==2){
				int u1=*graph[v.id].neighbor.begin();
				graph[v.id].neighbor.erase(u1);
				int u2=*graph[v.id].neighbor.begin();
				if(graph[u1].neighbor.count(u2)==1)
					maxans=3;
			}
			else {
				int u1=*graph[v.id].neighbor.begin();
				graph[v.id].neighbor.erase(u1);
				int u2=*graph[v.id].neighbor.begin();
				graph[v.id].neighbor.erase(u2);
				int u3=*graph[v.id].neighbor.begin();
				int a,b,c;
				a=graph[u1].neighbor.count(u2);
				b=graph[u1].neighbor.count(u3);
				c=graph[u2].neighbor.count(u3);
				if(a==1 && b==1 && c==1){
					printf("4\n");
					return 0;
				}
				if(a==1 || b==1 || c==1)
					maxans=3;

			}
			for(set<int>::iterator j=v.neighbor.begin();j!=v.neighbor.end();j++){
				degree[*j]--;
				graph[(*j)].neighbor.erase(v.id);
				if(degree[*j]<4)
					degree_queue.push(graph[*j]);
			}
		}

	}
	printf("%d\n", maxans);
	return 0;
}
