#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
const int N=41;
bool G[N][N]={{}};
int c[N]={};
int degree[N]={};
bool traversed[N];
int ans=N*100;
int V,E;
//bool operator<(int a,int b) { return degree[a]>degree[b]; }
int addup(){
/*	printf("===\n");
	for(int i=1;i<=V;i++)
		if(traversed[i])
			printf("%d ",i);
	printf("\n");
*/
	int sum=0;
	int added[N]={};
	for(int i=1;i<=V;i++){
		if(traversed[i]){
			if(!added[i]){
				sum+=c[i];
				added[i]=true;
			}			
			for(int j=1;j<=V;j++)
				if(G[i][j] && !added[j]){
					sum+=c[j];
					added[j]=true;
				}
		}
	}
/*	printf("added:\n");
	for(int i=1;i<=V;i++)
		if(added[i]) printf("%d\n",i );
	printf("\n");
*/
	return sum;
}
void DFS(int now){
	if(now==V){
		int sum=addup();
		if(sum<ans) ans=sum;
		return;
	}
	for(int i=1;i<=V;i++)
		if(G[now][i] && !traversed[i]){
			traversed[i]=true;
			DFS(i);
			traversed[i]=false;
		}
	return;
}

int main(){
	scanf("%d%d",&V,&E);
	for(int i=1;i<=V;i++){
		traversed[i]=false;
		scanf("%d",&c[i]);
	}
	int u,v;
	for(int i=0;i<E;i++){
		scanf("%d%d",&u,&v);
		G[u][v]=G[v][u]=true;
		degree[u]++; degree[v]++;
	}
	traversed[1]=true;
	DFS(1);
	printf("%d\n",ans );
	return 0;
}