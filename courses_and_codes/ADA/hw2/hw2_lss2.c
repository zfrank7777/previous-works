#include <stdio.h>
int lcs[2002][2002][2]={{{}}};
int main(){
	int N,M,K;
	scanf("%d%d%d",&N,&M,&K);
	int A[N];
	int B[M];
	for(int i=0;i<N;i++)
	scanf("%d",&A[i]);
	for(int i=0;i<M;i++)
		scanf("%d",&B[i]);

	for(int i=1;i<=M;i++)
		for(int j=1;j<=N;j++){
			if(A[j-1]==B[i-1])
				lcs[i][j][0] = lcs[i-1][j-1][0]+1;
			else
				lcs[i][j][0] = (lcs[i][j-1][0]>lcs[i-1][j][0])? lcs[i][j-1][0]:lcs[i-1][j][0];
		}
	int k;
	while(K){
		k=(k+1)%2;
		K--;
		for(int i=1;i<=M;i++)
			for(int j=1;j<=N;j++){
					if(A[j-1]==B[i-1])
						lcs[i][j][k] = lcs[i-1][j-1][k]+1;
					else{
						if(lcs[i][j-1][k]>lcs[i-1][j][k] && lcs[i][j-1][k]>lcs[i-1][j-1][(k+1)%2]+1)
							lcs[i][j][k] = lcs[i][j-1][k];
						else if(lcs[i][j-1][k]<lcs[i-1][j][k] && lcs[i-1][j][k]>lcs[i-1][j-1][(k+1)%2]+1)
							lcs[i][j][k] = lcs[i-1][j][k];
						else
							lcs[i][j][k] = lcs[i-1][j-1][(k+1)%2]+1;
					} //use k

				}
	}
/*	for(int k=0;k<=K;k++){
		for(int i=1;i<=M;i++){
			for(int j=1;j<=N;j++)
				printf("%d ",lcs[k][i][j]);
			printf("\n");
		}
		printf("\n\n");
	}
*/
	printf("%d\n", lcs[M][N][k]);

	return 0;
}