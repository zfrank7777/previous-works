#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
const int N=41;
bool G[N][N]={{}};
unsigned long long nei[N];
unsigned long long ONE=1;
int c[N]={};
int degree[N]={};
bool traversed[N];
int ans=N*100;
int V,E;
int addup(){
	int sum=0;
	int added[N]={};
	unsigned long long int add=0;
	for(int i=1;i<=V;i++)
		if(traversed[i])
			add |= nei[i];
	for(int i=1;i<=V;i++)
		if(add & (ONE<<i))
			sum+=c[i];
	return sum;
}
void DFS(int now, unsigned long long tr){
	if(now==V){
		int sum=addup();
		if(sum<ans) ans=sum;
		return;
	}
	for(int i=1;i<=V;i++)
		if(G[now][i] && !traversed[i] && !(tr & nei[i])){
			traversed[i]=true;
			DFS(i,tr | (ONE<<now));
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
		nei[u] |= ONE<<v;
		nei[v] |= ONE<<u;		
		degree[u]++; degree[v]++;
	}
	traversed[1]=true;
	DFS(1,0);
	printf("%d\n",ans );
	return 0;
}