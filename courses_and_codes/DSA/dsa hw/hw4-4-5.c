/*
可以求出關係的各個機器人放在同個group(R 1 2 W:group[1]=group[2]=1)
各個group之間用linklist連接
另外紀錄各個機器人的出拳(R 1 2 W:choose[1]=1,choose[2]=2)
*/
#include<stdio.h>
#include<stdlib.h>
#define MAX 1000001
int group[MAX]={};
int choose[MAX];
int group_index=1;
int size[MAX]={};

//given (a,b), return(b,a)
int converse(int x){
	if(x==1)
		return 2;
	if(x==2)
		return 1;
	if(x==3)
		return 3;
	if(x==4)
		return 4;
	return 0;
}

//given int r, return char r
char d_to_c(int x){
	if(x==1)
		return 'W';
	if(x==2)
		return 'L';
	if(x==3)
		return 'S';
	if(x==4)
		return 'N';
	return 'b';
}
//given char x, return int x
// 0:unknown, 1:win, 2:lose, 3:same, 4:nothing 
char c_to_d(char x){
	switch(x){
		case 'W':
			return 1;
			break;
		case 'L':
			return 2;
			break;
		case 'S':
			return 3;
			break;
		case 'N':
			return 4;
			break;
		default:
			return 0;
	}
}

//R x y relation
//given x y, return relation
// 0:unknown, 1:win, 2:lose, 3:same, 4:nothing 
int result(int x,int y){
	int k;
	k=(x>y)? (x-y):(x-y+4);
	if(k%4==0)
		return 3;
	if(k%4==1)
		return 2;
	if(k%4==2)
		return 4;
	if(k%4==3)
		return 1;
	printf("bug:123\n");
	return 0;
}

//R x y relation
//given x,relation, return y
//<relation> 0:unknown, 1:win, 2:lose, 3:same, 4:nothing 
//1>2>3>4>1
int match(int x,int relation){
	if(relation==0)
		return 0;
	if(x==0)
		return 0;
	int y;
	switch(relation){
		case 1:
			y=x+1;
			break;
		case 2:
			y=x+3;
			break;
		case 3:
			y=x;
			break;
		case 4:
			y=x+2;
			break;
		default:
			return 0;
	}
	return (y%4==0)? 4:(y%4);
}

typedef struct listnode
{
	int data;
	struct listnode* next;
}Node;
#define M 1000001
Node* sets[M];
Node* tail_list[M];

//merge group[x] and [y],size of x>y
void merge(int x,int y,int relation,int n){
	//connecting
	tail_list[group[x]]->next=sets[group[y]];
	tail_list[group[x]]=tail_list[group[y]];

	int k,delta,before,after;
	k=sets[group[y]]->data;
	before=choose[k];
	after=match(choose[x],relation);
	delta=after-before;
	size[group[x]]+=size[group[y]];
	while(size[group[y]]>0){
		size[group[y]]--;

		k=sets[group[y]]->data;

		group[k]=group[x];
		choose[k]+=delta;
		if(choose[k]<=0)
			choose[k]+=4;
		choose[k]%=4;
		if(choose[k]==0)
			choose[k]=4;	

		if(sets[group[y]]->next==NULL)
			break;		
		sets[group[y]]=sets[group[y]]->next;

	}
}
Node buffer[2*MAX];
int buf_index=0;

int main(){
	Node* temp;
	int T;
	scanf("%d",&T);
	for(int x=0;x<T;x++){
		int N,Q;
		scanf("%d%d",&N,&Q);
		// 0:unknown, 1:win, 2:lose, 3:same, 4:nothing 
		// 1>2>3>4>1
		buf_index=0;
		for(int i=0;i<M;i++){
			sets[i]=&buffer[buf_index];
			buf_index++;
			tail_list[i]=sets[i];
		}		
		for(int i=0;i<N+1;i++)
			size[i]=group[i]=choose[i]=0;
		for(int i=0;i<Q;i++){
			char R,type;	//R:R/A, type:WLSN
			int x,y,z;
			scanf("\n%c",&R);
			if(R=='R'){
				scanf("%d%d %c",&x,&y,&type);
				z=c_to_d(type);
				//printf("	z=%d\n",z);
				if(group[x]==0 && group[y]==0){
					group[x]=group[y]=group_index;
					size[group_index]+=2;
					group_index++;
					choose[x]=1;
					if(type=='S')
						choose[y]=1;
					else if(type=='N')
						choose[y]=3;
					else if(type=='W')
						choose[y]=2;
					else if(type=='L')
						choose[y]=4;
					else
						printf("bug:144\n");		
					printf("O\n");
					//printf("QQ\n");
					temp=&buffer[buf_index];
					buf_index++;
					tail_list[group_index]->data=x;
					tail_list[group_index]->next=temp;
					tail_list[group_index]=temp;

					temp=&buffer[buf_index];
					buf_index++;
					tail_list[group_index]->data=y;
					tail_list[group_index]->next=temp;
					tail_list[group_index]=temp;

				}
				else if(group[x]!=0 && group[y]==0){
					group[y]=group[x];
					choose[y]=match(choose[x],z);
					size[group[x]]++;
					printf("O\n");
					//printf("match:x=%d,y=%d,r=%d\n",choose[x],choose[y],z);
					temp=&buffer[buf_index];
					buf_index++;
					tail_list[group_index]->data=y;
					tail_list[group_index]->next=temp;
					tail_list[group_index]=temp;
				}
				else if(group[x]==0 && group[y]!=0){
					group[x]=group[y];
					choose[x]=match(choose[y],converse(z));
					size[group[y]]++;
					printf("O\n");
					temp=&buffer[buf_index];
					buf_index++;
					tail_list[group_index]->data=x;
					tail_list[group_index]->next=temp;
					tail_list[group_index]=temp;					
				}
				else if(group[x]!=0 && group[y]!=0 && group[x]!=group[y]){
					if(size[group[x]]>size[group[y]])
						merge(x,y,z,N);
					else
						merge(y,x,converse(z),N);
					printf("O\n");
				}
				else if(group[x]==group[y]){
					if(choose[y]==match(choose[x],z))
						printf("O\n");
					else
						printf("X\n");
				}
				else
					printf("bug:161\n");

			}
			else if(R=='A'){
				scanf("%d%d",&x,&y);
				if(group[x]==group[y] && group[x]!=0){
					z=result(choose[x],choose[y]);
					printf("%c\n",d_to_c(z));
				}
				else printf("?\n");
				//printf("%c,x=%d,y=%d,z=%d,q=%d\n",d_to_c(z),x,y,z,i+1);
			}

		}
		/*
		for(int i=1;i<N+1;i++)
			printf("%d,%d,%d\n",i,group[i],choose[i]);
		*/
	}
	return 0;
}

