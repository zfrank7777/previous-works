#include <stdio.h>
int main(){
	int N,M,K;
	scanf("%d%d%d",&N,&M,&K);
	int A[N];
	int B[M];
	for(int i=0;i<N;i++)
		scanf("%d",&A[i]);
	for(int i=0;i<M;i++)
		scanf("%d",&B[i]);

	int lcs[M+1][N+1];
	for(int i=0;i<=M;i++) lcs[i][0]=0;
	for(int i=0;i<=N;i++) lcs[0][i]=0;		
	for(int i=1;i<=M;i++)
		for(int j=1;j<=N;j++){
			if(A[j-1]==B[i-1])
				lcs[i][j] = lcs[i-1][j-1]+1;
			else
				lcs[i][j] = (lcs[i][j-1]>lcs[i-1][j])? lcs[i][j-1]:lcs[i-1][j];
		}

	printf("%d\n", lcs[M][N]);

	return 0;
}