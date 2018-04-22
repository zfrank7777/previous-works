#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
vector<int> odd;
vector<int> even;
int dis[100001];
int traversed[100001];
int odd_circle=0;
vector<int> adj_list[100001];
void DFS(int node,int parent,int traversed[100001],vector<int> adj_list[100001],int d){
	traversed[node]=1;
	dis[node]=d;
	for(int i=0;i<adj_list[node].size();i++){
		int u = adj_list[node][i];
		if(traversed[u]==0){
			if((d+1)%2==0)
				even.push_back(u);
			else
				odd.push_back(u);
			DFS(u,node,traversed,adj_list,(d+1)%2);
		}
		else if(odd_circle==0){
			if(dis[u]==(d)%2 && u!=parent)
				odd_circle=1;
		} 
	}
	return ;
}

int main(){
	int T;
	scanf("%d",&T);
	for(int t=0;t<T;t++){
		int V,E;
		scanf("%d%d",&V,&E);
		for(int i=0;i<=V;i++){
			adj_list[i].clear();
			dis[i]=traversed[i]=0;
		}
		for(int i=0;i<E;i++){
			int u,v;
			scanf("%d%d",&u,&v);
			adj_list[u].push_back(v);
			adj_list[v].push_back(u);
		}
		long long int bridge_added = 0;
		for(int i=1;i<=V;i++){
			odd_circle=0;
			if(traversed[i]==0){
				even.push_back(i);
				DFS(i,-1,traversed,adj_list,0);
				long long int odd_num = odd.size();
				long long int even_num = even.size(); 	
				if(odd_circle){
					bridge_added += (odd_num+even_num)*(odd_num+even_num-1)/2;
					//printf("odd_circle\n");
				}
				else{
					bridge_added += odd_num * even_num ;
					//printf("non odd_circle\n");
				}
				//printf("::%lld,%lld,%lld\n", bridge_added,odd_num,even_num);
				odd.clear();
				even.clear();
			}

		}
		bridge_added -= E;
		printf("%lld\n", bridge_added);
	}
	return 0;
}