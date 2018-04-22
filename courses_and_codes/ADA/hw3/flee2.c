#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <string.h>
#include <unistd.h>
using namespace std;
#define MAX 3000 

int main(){
	int R,C;
	scanf("%d%d",&R,&C);
	char house[MAX][MAX];
	char d_to_e[MAX][MAX]={{}};
	memset(d_to_e,-1,sizeof(d_to_e));
	int next[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
	char scanbuf[MAX];
	queue<int> x;
	queue<int> y;
	queue<int> Round;
	queue<int> px;
	queue<int> py;
	for(int i=0;i<R;i++){
		scanf("%s",&scanbuf);
		for(int j=0;j<C;j++){
			house[i][j]=scanbuf[j];
			if(house[i][j]=='E'){
				x.push(i);
				y.push(j);
				Round.push(0);
			}
			else if(house[i][j]=='P'){
				px.push(i);
				py.push(j);
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
		if(house[row][col]=='P' || house[row][col]=='.' || house[row][col]=='E') {
			if(d_to_e[row][col]<0)
				d_to_e[row][col]=round;
			else
				if(round>=d_to_e[row][col]) continue;
				else d_to_e[row][col]=round;
		}
		for(int k=0;k<4;k++){
			int r=row+next[k][0];
			int c=col+next[k][1];
			if(r<0 || r>=R || c<0 || c>=C)
				continue;
			if(house[r][c]=='F')
				continue;
			x.push(r);
			y.push(c);
			Round.push(round+1);
	//		fprintf(stderr,"add :%d %d\n",x.front(),y.front());
		}
	}
	while(!px.empty()){
		int pr=px.front();
		int pc=py.front();
		px.pop();
		py.pop();
		if(d_to_e[pr][pc]<0)
			printf("Died\n");
		else
			printf("%d\n", d_to_e[pr][pc]);
	}

	return 0;
}
