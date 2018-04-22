#include<stdio.h>
unsigned long long int step,L,R;
void move(int n,int from,int buffer,int to){
	//printf("__enter recursion__: step=%llu,n=%d\n",step,n);
	if(step>R || n==0)
		return;
	unsigned long long int temp=1;
	temp = temp <<(n-1);	
	if(step<L && L-step>temp-1 && temp!=1 && n<=64){
		//printf("skip:l-step=%llu,1<<n-1=%llu\n",L-step,temp);		
		step+=temp-1;
	}
	else{
		//printf("left:step=%llu,n=%d\n",step,n);
		move(n-1,from,to,buffer);
		
	}
	step++;
	if(step>=L && step<=R)
		printf("Step %llu: #%d -> #%d\n",step,from,to);
	if(step>=R)
		return;
	move(n-1,buffer,from,to);
	return;
}
int main(){
/*	int total[101];
	total[0]=total[1]=1;
	for(int i=2;i<=100;i++)
		total[i]=1+2*total[i-1];
*/
	int T;
	scanf("%d",&T);
	for(int i=0;i<T;i++){
		int n;
		step=0;
		scanf("%d%llu%llu",&n,&L,&R);
		printf("Case #%d:\n",i+1);
		move(n,1,2,3);
	}
	return 0;
}

