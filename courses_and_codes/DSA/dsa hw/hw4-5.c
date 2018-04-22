#include<stdio.h>
#include<stdlib.h>
#define MAX	1000
int queue[MAX+1];
int q_front;
int q_rear;
void q_insert(int x){
	queue[q_front]=x;
	if(q_front!=MAX-1)
		q_front++;
	else
		q_front=0;
}
int q_take(){
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
struct Node{
	struct Node* children[MAX];
	struct Node* parent;
	int number;
	int depth;
};
struct Node* new_node(int n,int d,char s[]){
	struct Node* new=malloc(sizeof(struct Node));
	new->number=n;
	new->depth=d;
	return new;
}
void BFS(struct Node* root,int p[][],int n,char color[][]){
	
	int now=root->number;
	for(int i=1;i<n;i++){
		if(p[now][i]==0 || color[i]!="WHITE")
			continue;
		struct Node* child=new_node(i,(root->depth)+1);
		q_insert(i);
		color[i]="GRAY";
		child->parent=root;
		root->children[i]=child;
	}
	color[now]="BLACK";
}
int main(){
	int T;
	scanf("%d",&T);
	for(int x=0;x<T;x++){
		q_front=q_rear=0;
		char color[n+1][5];
		int n,m,q;
		scanf("%d%d%d",&n,&m,&q);
		int portals[n+1][n+1];
		int choices[n+1][n+1];
		for(int i=0;i<n+1;i++){
			color[i]="WHITE";
			for(int j=0;j<n+1;j++)
				portals[i][j]=choices[i][j]=0;
		}
		struct Node* root=new_node(1,0);
		color[1]="GRAY";

		int a,b;
		for(int i=0;i<m;i++){
			scanf("%d%d",&a,&b);
			portals[a][b]=1;
		}
		for(int i=0;i<q;i++){
			scanf("%d%d",&a,&b);
			choices[a][b]=1;
		}
		for(int i=0;i<n+1;i++){
			for(int j=0;j<n+1;j++){

				printf("%2d",portals[i][j]);
			}
			printf("\n");
		}
	
			


	}
	return 0;
}