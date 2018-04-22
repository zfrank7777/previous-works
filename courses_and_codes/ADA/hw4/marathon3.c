#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
const int N=41;
bool G[N][N]={{}};
unsigned long long nei[N];
unsigned long long int tr=0;
unsigned long long ONE=1;
int c[N]={};
int degree[N]={};
bool traversed[N];
int n=0;
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
int min=2147483647;
int DFS(int now){
	traversed[now]=true;
	int sum = addup();
	if(sum>=min) return 2147483647;

	if(now==V)
		return (G[now][])? addup():2147483647;
		


	int tmp;
	for(int i=1;i<=V;i++)
		if(G[now][i] && !traversed[i]){
			tmp = DFS(i);
			min = (tmp<min)? tmp:min;
		}

	return min;
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

	ans = DFS(1);
	printf("%d\n",ans );
	return 0;
}