//uncompleted disjoint set
#include<stdio.h>
#include<stdlib.h>
struct element;
struct set;
struct element
{
	int data;
	struct element* next;
	struct set* class;
};
struct set
{
	struct element* head;
	struct element* tail;
	int type;
	int size;
};
struct set* make_set(int in){
	struct set* new = malloc(sizeof(struct set));
	struct element* e = malloc(sizeof(struct element));
	new->head=new->tail=e;
	new->type=0;
	new->size=1;
	e->data=in;
	e->class=new;
	e->next=NULL;
	return new;
}
void _union_(struct set* a,struct set* b){
	a->tail->next=b->head;
	a->tail=b->tail;
	a->size+=b->size;
	struct element* temp=b->head;
	while(temp!=NULL)
		temp->class=a;
	return ;
}

int main(){
	int T;
	scanf("%d",&T);
	printf("T=%d\n",T);

	for(int x=0;x<T;x++){
		int N,Q;
		scanf("%d%d",&N,&Q);
		printf("N=%d,Q=%d\n",N,Q);
		char table[N+1][N+1];
		for(int i=0;i<N+1;i++)
			for(int j=0;j<N+1;j++)
				table[i][j]='0';
		for(int y=0;y<Q;y++){
			char R,type;
			int x,y;
			scanf("\n%c",&R);
			// printf("R=%c\n",R);

			if(R=='R'){
				scanf("%d%d %c",&x,&y,&type);
				//printf("type=%c\n",type);
				if(1){
					if(type=='S' || type=='N')
						table[x][y]=table[y][x]=type;
					else if(type=='W'){
						table[x][y]='W';
						table[y][x]='L';
					}
					else{
						table[x][y]='L';
						table[y][x]='W';	
					}					
					printf("O\n");
				}
				else
					printf("X\n");

			}
			else if(R=='A'){
				scanf("%d%d",&x,&y);
				printf("%c\n",table[x][y]);
			}
			else
				printf("bug\n");
		}
	}
}