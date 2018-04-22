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
//lcs
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
//end of lcs
	int ans = lcs[M][N];
	int shorter_length = (M<N)? M:N;
	int sum=0;
	int temp;
	int row[lcs[M][N]+1];
	int col[lcs[M][N]+1];
	for(int i=0;i<=lcs[M][N];i++)	row[i]=col[i]=0;
	//if(K>shorter_length-lcs[M][N])	ans = shorter_length;
	if(K!=0){
		for(int i=1;i<=M;i++)
			if(lcs[i][N]==lcs[i-1][N])
				row[lcs[i][N]]++;
		for(int i=1;i<=N;i++)
			if(lcs[M][i]==lcs[M][i-1])
				col[lcs[M][i]]++;
		for(int i=0;i<=lcs[M][N];i++){
			temp = (row[i]<col[i])? row[i]:col[i];
			sum += temp;
		}
		temp = (sum>K)? K:sum;
		ans += temp;
	}
	printf("lcs matrix:\n");
	for(int i=1;i<=M;i++){
		for(int j=1;j<=N;j++)
			printf("%d ",lcs[i][j]);
		printf("\n");
	}
	printf("row array:\n");
	for(int j=0;j<=lcs[M][N];j++)
		printf("%d ",row[j]);
	printf("\n");	
	printf("col array:\n");
	for(int j=0;j<=lcs[M][N];j++)
		printf("%d ",col[j]);
	printf("\n");
	printf("K=%d,sum=%d\n", K,sum);

	printf("%d\n",ans);

	return 0;
}