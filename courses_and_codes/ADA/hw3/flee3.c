#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <string.h>
#include <unistd.h>
using namespace std;
#define MAX 3002
char house[MAX][MAX];
int d_to_e[MAX][MAX]={{}};

int main(){
	int R,C;
	scanf("%d%d",&R,&C);
	int next[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
	char scanbuf[MAX];
	queue<int> x;
	queue<int> y;
	queue<int> Round;
	for(int i=0;i<R;i++){
		scanf("%s",&house[i]);
		for(int j=0;j<C;j++){
			if(house[i][j]=='E'){
				x.push(i);
				y.push(j);
				Round.push(0);
			}
		}
	}			
	while(1){
		if(x.empty()) break;
		int row=x.front();
		int col=y.front();
		int round=Round.front();
		x.pop();
		y.pop();
		Round.pop();
	//	fprintf(stderr,"%d %d %d % d\n",x.empty(),y.empty(),row,col);
		for(int k=0;k<4;k++){
			int r=row+next[k][0];
			int c=col+next[k][1];
			if(r<0 || r>=R || c<0 || c>=C)
				continue;
			if(house[r][c]=='F' || d_to_e[r][c]!=0 || house[r][c]=='E')
				continue;
			d_to_e[r][c] = round+1;
			x.push(r);
			y.push(c);
			Round.push(round+1);

			//	fprintf(stderr,"add :%d %d\n",x.front(),y.front());
		}
	}
	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			if(house[i][j]!='P')
				continue;
			if(d_to_e[i][j]==0)
				printf("Died\n");
			else
				printf("%d\n", d_to_e[i][j]);
		}
	}

		

	return 0;
}