#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
 
unsigned long long int a, b, c, d;
unsigned long long int c_seed = 0, s_seed = 0; 
char cmd, s[5];
unsigned long long int Q,L,T;
 
unsigned long long int S=0;
unsigned long long int X=0;
unsigned long long int time=1;
unsigned long long int table[5];
 
void next_request (){
    //printf("%lld,%lld,%lld,%lld,%lld,%lld,%c,%s\n",c_seed,s_seed,a,b,c,d, cmd,s);
    c_seed = (c_seed * a + b) & 255;
    cmd = "AC"[c_seed & 1];
    for( int i = 0 ; i < L ; i ++ ){
        s_seed = (s_seed * c + d) & 1023;
        s[i] = (s_seed >> 3) & 63; 

    }
    //printf("%lld,%lld,%lld,%lld,%lld,%lld,%c,%s\n",c_seed,s_seed,a,b,c,d, cmd,s);

    if(cmd=='A')
        for(int i=0;i<L;i++){
            if((table[i] & (1<<s[i]))==0)
                table[i]+=(1<<s[i]);
        }
    else{
        int exist=1;
        for(int i=0;i<L;i++){
            if((table[i] & (1<<s[i]))==0)
                exist=0;
        }
        if(exist){
            S+=time;
            X^=time;
        }        
    }

    return;
}
unsigned long long int q[10001];
unsigned long long int A[10001][4];
int I=0;
int main(){
    scanf("%llu%llu%llu%llu%llu%llu%llu",&Q,&L,&T,&a,&b,&c,&d);
    for(int i=0;i<T;i++)
        scanf("%llu%llu%llu%llu%llu",&q[i],&A[i][0],&A[i][1],&A[i][2],&A[i][3]);
    while(Q--){
        if(time==q[I]){
            a=A[I][0];
            b=A[I][1];
            c=A[I][2];
            d=A[I][3];
            c_seed=s_seed=0;    
            I++;
        }
        next_request();
        time++;
    }
    printf("%llu %llu\n",S,X);
 
    return 0;
}