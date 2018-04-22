#include<stdio.h>
#include<stdlib.h>
#define MAX	1000
int queue[MAX+1];
int q_front;
int q_rear;
void q_push(int x){
	queue[q_front]=x;
	if(q_front!=MAX-1)
		q_front++;
	else
		q_front=0;
}
int q_pop(){
	if(q_front==q_rear){
		printf("queue empty\n");
		return -1;
	}
	int r=queue[q_rear];
	if(q_rear!=MAX-1)
		q_rear++;
	else
		q_rear=0;
	return r;
}
void BFS(int vertice[][3],int now,int n,edges[][]){
	int pcs;
	for(int i=0;i<vertice[now][2];i++){
		if(vertice[pcs]->color!=0)
			continue;
		vertice[i]->color=1;
		vertice[i]->depth=vertice[now]->depth+1;
		q_push(i);
	}
	vertice[now]->color=2;
}

int main(){
	int T;
	scanf("%d",&T);
	for(int x=0;x<T;x++){
		q_front=q_rear=0;
		int n,m,q;
		scanf("%d%d%d",&n,&m,&q);

		//initialize graph
		int depth[n+1];
		int color[n+1];
		int edge_index[n+1];
		int edges[n+1][MAX];
		for(int i=0;i<n+1;i++){
			depth[i]=MAX*MAX;
			color[i]=0;
			edge_index[i]=0;
		}

		int a,b;
		for(int i=0;i<m;i++){
			scanf("%d%d",&a,&b);
			edges[a][edge_index[a]]=b;
			edge_index[a]++;
		}
		for(int i=0;i<q;i++){
			scanf("%d%d",&a,&b);
		}

		int now;		
		while(vertice[1][1]!=2 || q_rear!=q_front){
			if(vertice[1][1]!=2)
				now=1;
			else
				now=q_pop();
			BFS(depth,color,edge_index,now,n);
		}
		printf("ans=%d\n",vertice[n][0]);


	}
	return 0;
}