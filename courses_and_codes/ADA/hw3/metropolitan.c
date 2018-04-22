#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;
vector<int> adj_list[200002];
struct tri
{
	long long int a;
	long long int b;
	long long int c;
	long long int n1;
	long long int n2;
	long long int n3;
};
vector<struct tri> three;
vector<struct tri> two;
void append(long long int x,long long int y,long long int z,int type){
	struct tri node;
	node.n1=x;
	node.n2=y;
	node.n3=z;
	node.a=x+y+z;
	node.b=x*y+y*z+x*z;
	node.c=x*y*z;
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
	for(long long int i=0;i<V;i++)
		adj_list[i].clear();
	for(long long int i=0;i<E;i++){
		scanf("%lld%lld",&u,&v);
		adj_list[u].push_back(v);
		adj_list[v].push_back(u);
		append(u,v,0,2);
	}

	for(long long int i=0;i<two.size();i++){	//1..2
		long long int v1=two[i].a;
		long long int v2=two[i].b;
		for(long long int j=0;j<V;j++){
			if(j==v1 || j==v2)
				continue;
			int havev1=0;
			int havev2=0;
			for(long long int x=0;x<adj_list[j].size();x++){
				long long int y=adj_list[j][x];
				if(y==v1) havev1=1;
				if(y==v2) havev2=1;
				if(havev1 && havev2){
					append(v1,v2,j,3);
					//printf("v1=%lld, v2=%lld, j=%lld\n", v1,v2,j);
					break;
				}
			}
		}
	} 
	if(three.empty()){
		printf("2\n");
		return 0;
	}
	int ans=3;
	for(long long int i=0;i<three.size();i++)
		for(long long int j=i+1;j<three.size();j++)
			if(three[i].a==three[j].a && three[i].b==three[j].b && three[i].c==three[j].c){
				three.erase(three.begin()+j);
				j--;
			}
	//for(long long int i=0;i<three.size();i++)
	//	printf("%d a=%lld b=%lld c=%lld\n", i,three[i].a,three[i].b,three[i].c);
	for(long long int i=0;i<three.size();i++){
		long long int u1=three[i].n1;
		long long int u2=three[i].n2;
		long long int u3=three[i].n3;
		for(long long int j=0;j<V;j++){
			if(j==u1 || j==u2 || j==u3)
				continue;
			long long int j12[3] = {j+u1+u2,j*u1+j*u2+u1*u2,j*u1*u2};
			long long int j13[3] = {j+u1+u3,j*u1+j*u3+u1*u3,j*u1*u3};
			long long int j23[3] = {j+u3+u2,j*u3+j*u2+u3*u2,j*u3*u2};
			int found=-2;
			for(int k=i+1;k<three.size();k++){
				if((three[k].a==j12[0] && three[k].b==j12[1] && three[k].c==j12[2])||
					(three[k].a==j23[0] && three[k].b==j23[1] && three[k].c==j23[2])||
					(three[k].a==j13[0] && three[k].b==j13[1] && three[k].c==j13[2])){
					found++;
					//printf("%d %lld %lld %lld \n", k,three[k].a,three[k].b,three[k].c);
				}
				if(found>0){
					ans=4;
					break;
				}
			}
		}
	}

	printf("%d\n",ans);
	return 0;
}