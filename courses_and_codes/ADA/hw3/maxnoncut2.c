#include <stdio.h>
#include <queue>
#include <vector>
using namespace std;

int main(){
	long long int V,E,s,t;
	scanf("%d%d%d%d",V,E,s,t);
	vector< vector< long long int > > adjacencyList(V + 1);
	vector< vector< long long int > > adjacencyListWeight(V + 1);
	long long int total_weight=0;
	for(int i=0;i<E;i++){
		long long int u,v,w;
		scanf("%lld%lld%lld",u,v,w);
		adjacencyList[u].push_back(v);
		adjacencyListWeight[u].push_back(w);
		total_weight += w;
	}
	queue<long long int> node;
	queue<long long int> weight;
	return 0;
}