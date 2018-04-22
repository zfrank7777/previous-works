#include<stdio.h>
#include<stdlib.h>
#define MAX	500001
int queue[MAX];
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
struct vertex{
	int depth;
	int edge_index;
	int edges[100];
	int color;	// 0=white,1=gray,2=black
};
struct vertex buffer[MAX];
int INDEX=0;
struct vertex* new_vertex(){
	struct vertex* new=&buffer[INDEX];
	INDEX++;
	new->depth=100000000;
	new->color=0;
	new->edge_index=0;
	return new;
}
void BFS(struct vertex* graph[],int n,int head){
	q_push(head);
	graph[head]->color=1;
	graph[head]->depth=0;
	int v,now;
	while(q_rear!=q_front){
		now=q_pop();
		for(int i=0;i<graph[now]->edge_index;i++){
			v=graph[now]->edges[i];
			if(graph[v]->color!=0)
				continue;
			graph[v]->color=1;
			graph[v]->depth=graph[now]->depth+1;
			q_push(v);
		}
		graph[now]->color=2;
	}
	return;
}
struct vertex* graph[MAX];
struct vertex* graph2[MAX];

int main(){
	int T;
	scanf("%d",&T);
	for(int x=0;x<T;x++){
		q_front=q_rear=0;
		int n,m,q;
		scanf("%d%d%d",&n,&m,&q);

		//initialize graph	
		for(int i=0;i<n+1;i++){
			graph[i]=new_vertex();
			graph2[i]=new_vertex();
		}

		int a,b;
		for(int i=0;i<m;i++){
			scanf("%d%d",&a,&b);
			graph[a]->edges[graph[a]->edge_index]=b;
			graph[a]->edge_index++;			
			graph2[b]->edges[graph2[b]->edge_index]=a;
			graph2[b]->edge_index++;
		}
		BFS(graph,n,1);
		q_front=q_rear=0;
		BFS(graph2,n,n);
		//int temp;

		for(int i=0;i<q;i++){
			scanf("%d%d",&a,&b);
			//printf("	%d,%d\n",graph[a]->depth,graph2[b]->depth);		
			if(graph[n]->depth < (graph[a]->depth+graph2[b]->depth+1))
				printf("%d\n",graph[n]->depth);
			else
				printf("%d\n",(graph[a]->depth+graph2[b]->depth+1));
		}
	}
	return 0;
}