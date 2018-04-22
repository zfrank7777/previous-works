#include<stdio.h>
void hanoi(int n,int from,int buffer,int to,
			unsigned long long int step,
			unsigned long long int L,
			unsigned long long int R){
	if(step>R)
		return;
	if(n==1){
		step++;
		//move from -> to
		if(L==1) printf("Step %llu: #%d -> #%d\n",step,from,to);
		return;
	}
	hanoi(n-1,from,to,buffer,step,L,R);
	step++;
	//move from -> to
	if(step>=L && step<=R)	
		printf("Step %llu: #%d -> #%d\n",step,from,to);
	hanoi(n-1,buffer,from,to,step,L,R);
	return;
}
int main(){
	unsigned long long int total_step[101];
	total_step[0]=total_step[1]=1;
	for(int i=2;i<=100;i++)
		total_step[i]=1+2*total_step[i-1];
	int T;
	scanf("%d",&T);
	for(int i=1;i<=T;i++){
		unsigned long long int l,r;
		int n;
		scanf("%d%llu%llu",&n,&l,&r);
		printf("Case #%d:\n",i);
/*		if(r<total_step[n]/2){
//			if(total_step[n-1]==total_step[n-1]+1)
//				break;
			hanoi(n-1,1,3,2,0,l,r);
		}
		else if(l<total_step[n]/2){
			printf("front half\n");
			hanoi(n-1,1,3,2,0,l,total_step[n]/2);
			printf("back half\n");
			hanoi(n,1,2,3,total_step[n]/2,total_step[n]/2,r);
		}
		else
*/		hanoi(n,1,2,3,1,l,r);
	}
	return 0;
}
