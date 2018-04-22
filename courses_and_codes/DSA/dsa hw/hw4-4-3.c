//正在改成struct pears,爛掉了

#include<stdio.h>
#include<stdlib.h>
#define MAX=1000001
struct pear{
	int first;
	int second;
	int result;
}
struct pear pears1[MAX];
struct pear pears2[MAX];
int pear1_index=0;
int pear1_index=0;

//given (a,b), return(b,a)
int converse(int r){
	switch(r){
		case 1:
			return 2;
		case 2:
			return 1;
		case 3:
			return 3;
		case 4:
			return 4;
		default:
			return 0;
	}
	return 0;
}

//a>b, create (a,b,r) in pears1, (b,a,converse(r)) in pears 2
void new_pears(int f, int s, int r){
	if(f>s){
		pears1[pear1_index]->first=f;
		pears1[pear1_index]->second=s;
		pears1[pear1_index]->result=r;		
		pears2[pear2_index]->first=s;
		pears2[pear2_index]->second=f;
		pears2[pear2_index]->result=converse(r);
	}
	else{
		pears1[pear1_index]->first=s;
		pears1[pear1_index]->second=f;
		pears1[pear1_index]->result=converse(r);		
		pears2[pear2_index]->first=f;
		pears2[pear2_index]->second=s;
		pears2[pear2_index]->result=r;
	}
	pear1_index++;
	pear2_index++;		
	return;
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
	return '?';
}

//given (a,b)=x (b,c)=y, return(a,c)
int trans(int x,int y){
	if(x==3)
		return y;
	if(y==3)
		return x;
	if(x==1)
		switch(y){
			case 1:
				return 4;
			case 2:
				return 3;
			case 4:
				return 2;
		}
	if(x==2)
		switch(y){
			case 1:
				return 3;
			case 2:
				return 4;
			case 4:
				return 1;
		}
	if(x==4)
		switch(y){
			case 1:
				return 2;
			case 2:
				return 1;
			case 4:
				return 3;
		}
	return 0;
}

void fill(int x,int y,int z){
	if(z==3){
		table[x][y]=table[y][x]=3;
	}
	else if(z==4){
		table[x][y]=table[y][x]=4;
	}
	else if(z==1){
		table[x][y]=1;
		table[y][x]=2;
	}
	else{
		table[x][y]=2;
		table[y][x]=1;	
	}	
	return;
}



int main(){
	int T;
	scanf("%d",&T);
	for(int x=0;x<T;x++){
		int N,Q;
		scanf("%d%d",&N,&Q);
		int table[N+1][N+1];	// 0:unknown, 1:win, 2:lose, 3:same, 4:nothing 
		for(int i=0;i<N+1;i++)
			for(int j=0;j<N+1;j++)
				table[i][j]=0;
		for(int y=0;y<Q;y++){
			char R,type;
			int x,y,z;
			scanf("\n%c",&R);
			if(R=='R'){
				scanf("%d%d %c",&x,&y,&type);
				if(table[x][y]==0){
					if(type=='S')
						z=3;
					else if(type=='N')
						z=4;
					else if(type=='W')
						z=1;
					else
						z=2;
					new_pear(x,y,);
					for(int i=1;i<N+1;i++)
						if(table[i][x]!=0){
							z=trans(table[i][x],table[x][y]);
							fill(table,i,y,z);
						}				
					printf("O\n");
				}
				else
					printf("X\n");

			}
			else if(R=='A'){
				scanf("%d%d",&x,&y);
				printf("%c\n",d_to_c(table[x][y]));
			}
		}
	}
	return 0;
}

