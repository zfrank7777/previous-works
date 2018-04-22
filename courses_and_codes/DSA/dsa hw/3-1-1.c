#include<stdio.h>

int main(){
	int arr[11]={};
	int in[7]={18, 34, 9, 37, 40, 32, 89};
	int h1,h2;
	printf("zero represent nothing\n");
	for(int i=0;i<7;i++){
		h1=in[i]%11;
		while(arr[h1]){
			h1++;
			if(h1==11)
				h1=0;
		}
		arr[h1]=in[i];
		printf("round %d	 |",i+1);
		for(int j=0;j<11;j++){
			printf("%2d|",arr[j]);
		}
		printf("\n");

	}
	return 0;
}