#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

char ips[2000000][32];
char request[3000000][32];

struct Node{
	struct Node *one;
	struct Node *zero;		
};
struct Node *new_node(){
	struct Node *new=malloc(sizeof(struct Node));
	new->one=NULL;
	new->zero=NULL;
	return new;
}

int isleaf(struct Node* z){
	if(z->one==NULL && z->zero==NULL)
		return 1;
	return 0;
}

void d_to_b(char S[32],int n1,int n2, int n3,int n4){
	for(int i=0;i<8;i++){
		S[i]=n4%2;
		n4=n4>>1;
	}
	for(int i=8;i<16;i++){
		S[i]=n3%2;
		n3=n3>>1;
	}	
	for(int i=16;i<24;i++){
		S[i]=n2%2;
		n2=n2>>1;
	}	
	for(int i=24;i<32;i++){
		S[i]=n1%2;
		n1=n1>>1;
	}	
}

int main(){
	int T;
	scanf("%d",&T);
	for (int i=0;i<T;i++){
		int M,N;
		int n1,n2,n3,n4,digit;
		scanf("%d%d",&M,&N);
		struct Node *trie=new_node(); 
		struct Node *root = trie;		
		for(int j=0;j<M;j++){
			scanf("%d.%d.%d.%d/%d",&n1,&n2,&n3,&n4,&digit);
			d_to_b(ips[j], n1,n2,n3,n4);
			for(int k=31;k>=(32-digit);k--){
				//printf("%d",ips[j][k]);
				if(ips[j][k]==1){
					if(trie->one==NULL)
						trie->one=new_node();
					trie=trie->one;
				}
				else{
					if(trie->zero==NULL)
						trie->zero=new_node();
					trie=trie->zero;
				}
				if(k==32-digit){
					if(trie->one)
						trie->one=NULL;
					if(trie->zero)
						trie->zero=NULL;
				}
			}
			trie=root;
		}
		/*
		trie=root;
		for(int k=31;k>=0;k--){
			printf("%d\n", k);
			if(isleaf(trie)){
				printf("end\n");
				break;
			}
			
			if(trie->one){
				printf("1\n");
				if(trie->zero)
					printf("wrong:both\n");
				else
					trie=trie->one;

			}
						
			else {
				printf("0\n");
				trie=trie->zero;
			}				
		}*/
		for(int j=0;j<N;j++){
			scanf("%d.%d.%d.%d",&n1,&n2,&n3,&n4);
			d_to_b(request[j],n1,n2,n3,n4);
			for(int k=31;k>=0;k--){
				//printf("%d",request[j][k]);
				if(isleaf(trie)){
					printf("TRUE\n");
					break;
				}
				if(request[j][k]==1){
					if(trie->one==NULL){
						printf("FALSE\n");
						break;
					}
					trie=trie->one;
				}
				else{
					if(trie->zero==NULL){
						printf("FALSE\n");
						break;
					}
					trie=trie->zero;
				}
			}
			trie=root;
		}
		/*for(int k=0;k<32;k++){
			for(int j=0;j<M;j++)
				ips[j][k]=' ';
			for(int j=0;j<N;j++)
				request[j][k]=' ';
		}*/
	}
	return 0;
}