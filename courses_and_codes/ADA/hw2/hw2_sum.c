#include <stdio.h>
int main(){
	unsigned long long int T,n,k;
	scanf("%llu",&T);
	for(int i=0;i<T;i++){
		scanf("%llu%llu",&n,&k);
		unsigned long long int temp=k;
		unsigned long long int input[n];
		unsigned long long int output[n];
		unsigned long long int out_index=0;
		unsigned long long int m;
		for(int j=1;j<=n;j++)
			scanf("%llu",&input[j]);
		for(int j=n;j>0;j--){
			if(input[j]>temp)
				continue;
			else{
				output[out_index]=input[j];
				out_index++;
				temp-=input[j];
			}
			//printf("temp=%d\n",temp);
			if(temp<=0)
				break;			
		}
		if(out_index==0 || temp>0){
			printf("-1\n");
			continue;
		}
		printf("%llu\n",out_index);
		for(int j=out_index-1;j>=0;j--)
			printf("%llu ",output[j]);
		printf("\n");
		
	}
	return 0;
}