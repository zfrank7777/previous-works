// O(n^2) memory: killed by online judge
#include<stdio.h>
#include<stdlib.h>
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
int trans(int x,int y){
	//(a,b)=x (b,c)=y, return(a,c)
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
					fill(table,x,y,z);
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

