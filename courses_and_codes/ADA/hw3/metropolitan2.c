#include <stdio.h>
#include <string.h>
#include <vector>
#include <queue>
using namespace std;
vector<int> adj_list[200002];
struct tri
{
	long long int a;
	long long int b;
	long long int c;
};
queue<struct tri> three;
queue<struct tri> two;
void append(long long int x,long long int y,long long int z,int type){
	struct tri node;
	node.a=x;
	node.b=y;
	node.c=z;
	if(type==3)
		three.push_back(node);
	else
		two.push_back(node);
	return;
}

int main(){
	long long int V,E;
	scanf("%lld%lld",&V,&E);
	if(E==0){
		printf("1\n");
		return 0;
	}
	long long int u,v;
	two.clear();
	three.clear();
	for(long long int i=0;i<E;i++){
		adj_list[i].clear();
		scanf("%lld%lld",&u,&v);
		adj_list[u].push_back(v);
		adj_list[v].push_back(u);
		append(u,v,0,2);
	}

	for(long long int i=0;i<two.size();i++){	//1..2
		long long int n1=two[i].a;
		long long int n2=two[i].b;
		for(long long int j=0;j<V;j++){
			if(j==n1 || j==n2)
				continue;
			int haven1=0;
			int haven2=0;
			for(long long int x=0;x<adj_list[j].size();x++){
				long long int y=adj_list[j][x];
				if(y==n1) haven1=1;
				if(y==n2) haven2=1;
				if(haven1 && haven2){
					append(n1+n2+j,n1*n2+n1*j+n2*j,n1*n2*j,3);
					printf("n1=%lld, n2=%lld, j=%lld\n", n1,n2,j);
					break;
				}
			}
		}
	} 
	if(three.empty()){
		printf("2\n");
		return 0;
	}
	for (int i = 0; i < three.size(); ++i)
		printf("%d a=%lld b=%lld c=%lld\n", i,three[i].a,three[i].b,three[i].c);
	int ans=3;
	for(long long int i=0;i<three.size();i++){
		int temp=0;
		for(long long int j=i+1;j<three.size();j++){
			if(three[i].a==three[j].a && three[i].b==three[j].b && three[i].c==three[j].c){
				temp++;
				printf("%d %lld %lld\n",temp,i,j);
			}
			if(temp==3){
				ans=4;
				break;
			}
		}
	}
	printf("%d\n",ans);
	return 0;
}