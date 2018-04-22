#include <stdio.h>
#include <math.h>
typedef unsigned long long ll;
static ll step;
void hanoi(int n,int start,int end,int buffer,ll l,ll r,ll low,ll high){
	if(n==1){
		if(step>=l&&step<=r){
			printf("Step %llu: #%d -> #%d\n",step,start,end);
		}
		step++;
		return;
	}
	ll left_low = low;
	ll left_high = (high/2);
	ll right_low = (high/2)+2;
	ll right_high = high;
	if(left_high >= l || left_low<= r)
		hanoi(n-1,start,buffer,end,l,r,left_low,left_high);
	if(step>=l&&step<=r){
		printf("Step %llu: #%d -> #%d\n",step,start,end);
	}
	step++;
	if(right_high >= l || right_low<= r)
		hanoi(n-1,buffer,end,start,l,r,right_low,right_high);
	return;
}
int main(){
	int scenarios;
	scanf("%d",&scenarios);
	int i;
	for(i=0;i<scenarios;i++){
		step=1;
		int n;	
		ll l,r;
		scanf("%d",&n);
		scanf("%llu",&l);
		scanf("%llu",&r);
		printf("Case #%d:\n",i+1);
		ll total;
		int start=1,end=3,buffer=2;
		if(n<63){
			total= (pow(2,n)-1);
		}else if(n==63 || n%2==1){
			total =(ll)9223372036854775807;
			n=63;
		}else{
		    total =(ll)9223372036854775807;
			n=63;
			end=2;
			buffer=3;
		}
		hanoi(n,start,end,buffer,l,r,1,total);
	}
}
