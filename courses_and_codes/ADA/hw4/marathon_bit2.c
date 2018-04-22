#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
const int N=41;
unsigned long long int G[N]={};
bool adj_matrix[N][N]={{}};
int c[N]={};
int degree[N]={};
bool traversed[N];
int ans=N*100;
int V,E;
//bool operator<(int a,int b) { return degree[a]>degree[b]; }
int addup(){
	int sum=0;
	unsigned long long int added=0;
	for(int i=1;i<=V;i++)
		if(traversed[i]){
			added |= (unsigned long long)(1<<i);
			added |= G[i];
		}
	for(int i=1;i<=V;i++)
		if((added>>i)%2==1)
			sum+=c[i];
	return sum;
}
void DFS(int now){
	if(now==V){
		int sum=addup();
		if(sum<ans) ans=sum;
		return;
	}
	for(int i=1;i<=V;i++)
		if(adj_matrix[now][i] && !traversed[i]){
			traversed[i]=true;
			DFS(i);
			traversed[i]=false;
		}
	return;
}
int main(){
	scanf("%d%d",&V,&E);
	for(int i=1;i<=V;i++)
		scanf("%d",&c[i]);
	int u,v;
	for(int i=0;i<E;i++){
		scanf("%d%d",&u,&v);
		adj_matrix[u][v]=adj_matrix[v][u]=true;
		G[u] |= (unsigned long long)1<<v;
		G[v] |= (unsigned long long)1<<u;
		degree[u]++; degree[v]++;
	}
	traversed[1]=true;
	DFS(1);
	printf("%d\n",ans );
	return 0;
}