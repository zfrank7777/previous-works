#include <stdio.h>

int main(){
	char c;
	int S[101];
	S[0]=0;
	int S_index=1;
	scanf("%c",&c);
	int sum=0;
	int q_num=0;
	while(c!='\n'){
		printf("%c\n", c);
		if(c=='(')	S[S_index]=1;
		else if (c=='?'){
			S[S_index]=0;
			q_num++;
		}	
		else		S[S_index]=-1;
		sum+=S[S_index];
		S_index++;
		if( sum<-q_num ){
			printf("impossible\n");
			return 0;
		}		
		scanf("%c",&c);
	}
	if(sum>q_num || sum<-q_num || ((sum+q_num)%2!=0)){
		printf("impossible\n");
		return 0;
	}	
	S_index--;
	int Lcost[q_num+1];
	int Rcost[q_num+1];
	for(int i=1;i<=q_num;i++)
		scanf("%d%d",&Lcost[i],&Rcost[i]);

	int total_cost=0;
	for(int i=1;i<=q_num;i++){
		if(sum>0) {
			total_cost+=Rcost[i];
			sum--;
		}
		else{
			total_cost+=Lcost[i];
			sum++;
		}
	}
	if(sum!=0)
		printf("impossible\n");
	else
		printf("%d\n", total_cost);
/*	printf("%d,%d\n", sum,S_index);
	for(int i=0;i<=S_index;i++)
		printf("%d ", S[i]);
	printf("\n");
*/
	return 0;
}