//start out over again
//choose: 0~3,initialize(-1)
#include<stdio.h>
#include<stdlib.h>
#define MAX 1000001
//array:group&choose
int group[MAX]={};
int choose[MAX]={};
int size[MAX]={};
//linklist:for union
typedef struct listnode{
	int data;
	struct listnode* next;
}Node;
Node* head[MAX];
Node* tail[MAX];
//only called in function match
char accurate_match(int x,int y){
	char r;
	switch(x-y){
		case 0:
			r='S';
			break;
		case 1:
			r='L';
			break;
		case 2:
			r='N';
			break;
		case 3:
			r='W';
			break;
		default:
			r='+';
	}	
	return r;
}
//given x,y return result
char match(int first,int second){
	if(group[first]!=group[second] || choose[first]==-1 || choose[second]==-1)
		return '?';
	char r;
	if(choose[first]>=choose[second])
		r=accurate_match(choose[first],choose[second]);
	else
		r=accurate_match(choose[first]+4,choose[second]);
	return r;
}
//add b to a
//WLSN=3102
void group_union(int a,int b,int delta){
	size[a]+=size[b];
	tail[a]->next=head[b];
	tail[a]=tail[b];
	int v;
	//printf("size[b]=%d\n",size[b]);
	for(int x=0;x<size[b];x++){
		//printf("x=%d\n", x);
		v=head[b]->data;
		group[v]=a;
		//int u=choose[v];
		choose[v]=(choose[v]+delta)%4;
		//printf(" v:%d,before:%d,after:%d,delta:%d\n",v,u,choose[v],delta);
		if(head[b]->next!=NULL)
			head[b]=head[b]->next;
	}
	size[b]=0;
	return;
}
//r(a,b)=(a-b+4)%4
//WLSN=3102
int c_to_d(char x){
	switch(x){
		case 'W':
			return 3;
			break;
		case 'L':
			return 1;
			break;
		case 'S':
			return 0;
			break;
		case 'N':
			return 2;
			break;
		default:
			return -1;
	}
}
void quest(int first,int second,char result){
	int z=c_to_d(result);
	//printf("confirm:%d=%c\n",z,result);
	if(choose[second]<0){
		if(choose[first]<0)
			choose[first]=0;
		choose[second]=(choose[first]-z+4)%4;
		group_union(group[first],second,0);
		printf("O\n");
	}
	else if(choose[first]<0){
		choose[first]=(choose[second]+z)%4;
		group_union(group[second],first,0);
		printf("O\n");
	}
	else{
		if(group[first]!=group[second]){
			int should_be;
			int dt;
			if(size[ group[first] ] > size[ group[second] ]){
				should_be=(choose[first]-z+4)%4;
				dt=(should_be-choose[second]+4)%4;
				group_union(group[first],group[second],dt);
			}
			else{
				should_be=(choose[second]+z)%4;
				dt=(should_be-choose[first]+4)%4;
				group_union(group[second],group[first],dt);
			}
			printf("O\n");	
		}
		else{
			if(result==match(first,second))
				printf("O\n");
			else
				printf("X\n");
		}

	}
	return;
}
Node buffer[MAX];
int b_index;

int main(){
	int T;
	scanf("%d",&T);
	for(int i=0;i<T;i++){
		int N,Q;
		scanf("%d%d",&N,&Q);
		//initialize array & linklist
		b_index=0;
		for(int k=0;k<N+1;k++){
			group[k]=k;
			choose[k]=-1;
			size[k]=1;
			head[k]=&buffer[b_index];
			head[k]->data=k;
			head[k]->next=NULL;
			b_index++;
			tail[k]=head[k];
		}
		//printf("===%d===\n",i );
		for(int j=0;j<Q;j++){
			char command,result;
			int first,second;
			scanf("\n%c%d%d",&command,&first,&second);
			if(command=='R'){
				scanf(" %c",&result);
				quest(first,second,result);
			}
			else if(command=='A')
				printf("%c\n",match(first,second));
			else
				printf("bug:wrong command\n");
			result='Q';
		}
		/*while(head[5]->data!=tail[5]->data){
			int j=head[5]->data;
			printf("now:%d,group=%d,choose=%d,size=%d\n",j,group[j],choose[j],size[j]);
			head[5]=head[5]->next;
		}*/
	}
	return 0;
}