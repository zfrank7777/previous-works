#include<stdio.h>
unsigned long long int temp[2][2];
unsigned long long int mtx[30][2][2];
void matrix_square(unsigned long long int i){
	temp[0][0]= (mtx[i][0][0]*mtx[i][0][0]) %1000000007 +	
				(mtx[i][0][1]*mtx[i][1][0]) %1000000007;
	temp[0][1]=	(mtx[i][0][0]*mtx[i][0][1]) %1000000007 +
				(mtx[i][0][1]*mtx[i][1][1]) %1000000007;
	temp[1][0]= (mtx[i][1][0]*mtx[i][0][0]) %1000000007 +	
				(mtx[i][1][1]*mtx[i][1][0]) %1000000007;
	temp[1][1]=	(mtx[i][1][0]*mtx[i][0][1]) %1000000007 +
				(mtx[i][1][1]*mtx[i][1][1]) %1000000007;	
	return ;
}

int main(){	
	mtx[0][0][0]=0;
	mtx[0][0][1]=mtx[0][1][0]=mtx[0][1][1]=1;
	unsigned long long int exp[31];
	exp[0]=1;
	for(int i=1;i<=30;i++){
		exp[i]=exp[i-1]*2;
		matrix_square(i-1);//temp = sqr of mtx[i-1]
		mtx[i][0][0]=temp[0][0];mtx[i][0][1]=temp[0][1];
		mtx[i][1][0]=temp[1][0];mtx[i][1][1]=temp[1][1];
	}
	int T;
	scanf("%d",&T);
	for(int i=0;i<T;i++){
		unsigned long long int f1,f2,n,m00,m01,m10,m11;;
		unsigned long long int m[2][2]={{1,0},{0,1}};
		scanf("%lld%lld%lld",&f1,&f2,&n);
		for(int j=0;j<=30;j++){
			if(exp[j]&(n-1)){
				m00 =   (m[0][0]*mtx[j][0][0])%1000000007+
						(m[0][1]*mtx[j][1][0])%1000000007;
				m01 =   (m[0][0]*mtx[j][0][1])%1000000007+
						(m[0][1]*mtx[j][1][1])%1000000007;
				m10 =   (m[1][0]*mtx[j][0][0])%1000000007+
						(m[1][1]*mtx[j][1][0])%1000000007;
				m11 =   (m[1][0]*mtx[j][0][1])%1000000007+
						(m[1][1]*mtx[j][1][1])%1000000007;
				m[0][0]=m00;m[0][1]=m01;
				m[1][0]=m10;m[1][1]=m11;
			}		
		}
		unsigned long long int ans =   (f1*m[0][0])%1000000007+
					(f2*m[1][0])%1000000007;
		ans %= 1000000007;;
		printf("%llu\n",ans);		
	}
	return 0;
}
