#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
long long int network_mask[2000010] = {0} ;
int number[2000010] = {0} ;
int main(void)
{
    
    int n ; 
    scanf("%d" , &n);
    int M , N;
    long long int i1 , i2 , i3 , i4;
    long long int i11 , i22 , i33 , i44;
    for(int i = 0; i < n; i++)
    {
       
        scanf("%d %d" , &M , &N);
        for(int i = 0 ; i < M ; i++)
        {
            scanf("%lld.%lld.%lld.%lld/%d" , &i1 , &i2 , &i3 , &i4 , &number[i]) ;
            long long int a1 = i1, a2 = i2, a3 = i3, a4 = i4;
            network_mask[i] = a1 * 256 * 256 * 256 + a2 * 256 * 256 + a3 * 256 + a4;
            //printf("%lld", network_mask[i]);
        }
        for(int i = 0; i < N; i++)
        {
            scanf("%lld.%lld.%lld.%lld" , &i11 , &i22 , &i33 , &i44) ;
            long long int trans;
            long long int a11 = i11, a22 = i22, a33 = i33, a44 = i44;
            trans = a11 * 256 * 256 * 256 + a22 * 256 * 256 + a33 * 256 + a44;
            //printf("%lld", trans);
;            int flag = 0;
            for(int i = 0 ; i < M ; i++)
            {
                trans = trans >> (32 - number[i]) ;
                long long int comp = (trans == network_mask[i] >> (32 - number[i]));
                if(comp)
                {
                    flag = 1;
                    printf("TRUE\n");
                    break;
                }
            }
            if(flag == 0)
                printf("FALSE\n");
        }
    }
    return 0 ;
}