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
///
void qpush(int n,int d){
	graph[n].d=d;
	graph[n].id=n;
	graph[n].neighbor=adj_list[n];
	degree_queue.push(graph[n]);
	return;
}
void record(int n,int d){
	graph[n].d=d;
	graph[n].id=n;
	graph[n].neighbor=adj_list[n];
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
		adj_list[v1].insert(v2);
		adj_list[v2].insert(v1);
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
		if(degree[v.id]<3 && maxans==3){
			for(set<int>::iterator j=v.neighbor.begin();j!=v.neighbor.end();j++){
				degree[*j]--;
				graph[(*j)].neighbor.erase(graph[(*j)].neighbor.find(v.id));
			}
			continue;
		} 
		set<int>::iterator iter=v.neighbor.begin();
		for(int j=0;j<2;j++,iter++){
			int u = *(iter);
			//printf("%d %d\n", v.id,u);
			set<int>::iterator it=iter;
			it++;
			for(int k=j+1;k<v.d;k++,it++){
				int u2=*(it);
				if(done[u2]>0 || u2==i)
					continue;
				if(graph[u2].neighbor.count(u)==1){
					//printf("%d %d %d\n", v.id,u,u2);
					maxans=3;
				}
				else
					fourflag=0;	
			}
			//printf("ans=%d\n",maxans );
		}
		if(fourflag){
			printf("4\n");
			return 0;
		}

		for(set<int>::iterator j=v.neighbor.begin();j!=v.neighbor.end();j++){
			degree[*j]--;
			graph[(*j)].neighbor.erase(graph[(*j)].neighbor.find(v.id));
		}
/*		v.neighbor.clear();
		printf("////\n");
		for(int z=0;z<V;z++){
			for(set<int>::iterator q=graph[z].neighbor.begin();q!=graph[z].neighbor.end();q++)
				printf("%d ", *q);
			printf("\n");
		}
		printf("====\n");
*/
	}
	printf("%d\n", maxans);
	return 0;
}