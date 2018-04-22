#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <string.h>
#include <unistd.h>
using namespace std;
#define MAX 3000 
int BFS(queue<int> x,queue<int> y,char House[MAX][MAX],int R,int C){
	if(x.empty()) return -1;
	int i=x.front();
	x.pop();
	int j=y.front();
	y.pop();
	fprintf(stderr,"%d %d %d %d\n",x.empty(),y.empty(),i,j);
	if(House[i][j]=='E') return 0;
	House[i][j]='X';
	if(i!=0 && House[i-1][j]!='F' && House[i-1][j]!='X'){
		x.push(i-1);
		y.push(j);
		fprintf(stderr,"add up\n");
	}
	if(j!=0 && House[i][j-1]!='F' && House[i][j-1]!='X'){
		x.push(i);
		y.push(j-1);
		fprintf(stderr,"add left\n");
	}
	if(i!=R-1 && House[i+1][j]!='F' && House[i+1][j]!='X'){
		x.push(i+1);
		y.push(j);
		fprintf(stderr,"add down\n");
	}
	if(j!=C-1 && House[i][j+1]!='F' && House[i][j+1]!='X'){
		x.push(i);
		y.push(j+1);
		fprintf(stderr,"add right:%c, %d %d\n",House[i][j+1],x.front(),y.front());
	}	
	return 1;
}

int main(){
	int R,C;
	scanf("%d%d",&R,&C);
	char house[MAX][MAX];
	char temp_house[MAX][MAX];
	int next[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
	char scanbuf[MAX];
	for(int i=0;i<R;i++){
		scanf("%s",&scanbuf);
		for(int j=0;j<C;j++)
			house[i][j]=scanbuf[j];
	}			
	for(int i=0;i<R;i++)
		for(int j=0;j<C;j++){
			if(house[i][j]!='P')	
				continue;		
			queue<int> x;
			queue<int> y;
			memset(temp_house,0,sizeof(temp_house));
			memcpy(temp_house,house,sizeof(house));
			x.push(i);
			y.push(j);
			//fprintf(stderr,"%d %d\n",x.front(),y.front());
			int step=0;
			while(1){
				if(x.empty()){
					step=-1;
					break;
				}
				int row=x.front();
				x.pop();
				int col=y.front();
				y.pop();
			//	fprintf(stderr,"%d %d %d %d\n",x.empty(),y.empty(),row,col);
				if(temp_house[row][col]=='E'){
					step += (i-row>0)? i-row:row-i;
					step += (j-col>0)? j-col:col-j;
					break;
				}
				temp_house[row][col]='X';
				for(int k=0;k<4;k++){
					int r=row+next[k][0];
					int c=col+next[k][1];
					if(r<0 || r>=R || c<0 || c>=C)
						continue;
					if(temp_house[r][c]=='F' || temp_house[r][c]=='X')
						continue;
					x.push(r);
					y.push(c);
			//		fprintf(stderr,"add :%d %d\n",x.front(),y.front());
				}
			}

			if(step!=-1)
				printf("%d\n", step);
			else
				printf("Died\n");
		}

	return 0;
}
