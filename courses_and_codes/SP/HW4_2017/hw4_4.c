//4_3: building tree
//4_4: array
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>

#define Feature_DIM 33
#define Training_NUM 25150

double dataset[Training_NUM][Feature_DIM+2];
double rand_data[Training_NUM][Feature_DIM+2];

static int cmp(const void *a, const void *b,  void *D){
      int c = *(int *)a;
      int d = *(int *)b;
      int f = *(int *)D;
      if(dataset[c][f] < dataset[d][f])
      	{return -1;}               //傳回 -1 代表 a < b
      else if (dataset[c][f] == dataset[d][f]) 
      	{return 0;}     			 //傳回   0 代表 a = b
      else return 1;                 //傳回  1 代表 a>b
}

typedef struct Node{
	int feature;
	double impurity;
	int cut;
	struct Node* left;
	struct Node* right;
	int isleaf;
	int label;
}node;

void plant(int N, double** data,node* now){
	int total_one=0;
	double min_imp=2;
	int min_cut,min_f,count;	
	int sorted_id[N];
	int sorted_count[N];
	for(int i=0;i<N;i++){
		if(data[i][Feature_DIM]==1)	total_one++;
		sorted_id[i]=data[i][0];
	}
	//end condition
	if(total_one==0 || total_one==N)
		return;
	//sort & calculate min impurity
	for(int f=1;f<=Feature_DIM;f++){
		qsort_r(sorted_id,N,sizeof(int),cmp,&f);
		count = 0;
		for(int i=0;i<N;i++){
			if(data[sorted_id[i]][Feature_DIM]==0) count++;
			sorted_count[i]=count;
		}
		double imp, denom, numer;
		for(int i=0;i<N-1;i++){
			imp = 0;
			denom = (i+1);numer = sorted_count[i];imp += (numer/denom)*(1-(numer/denom));
			denom = (N-i-1);numer = (count-sorted_count[i]);imp += (numer/denom)*(1-(numer/denom));
			if(imp < min_imp){min_imp = imp;min_cut = i;min_f = f;}
		}
	}
	printf("%d %lf %d\n",min_f,min_imp,min_cut);	
	now->feature = min_f;
	now->impurity = min_imp;
	now->cut = min_cut;
	now->right = (node*)malloc(sizeof(node));
	now->left = (node*)malloc(sizeof(node));
	//build children
	qsort_r(sorted_id,N,sizeof(int),cmp,&min_f);
	//plant(min_cut,)

	return;
}

int main(int argc,char** argv){
	FILE* training_file = fopen("training_data","r");
	int id;
	memset(dataset,0,sizeof(dataset));
	for(int i=0;i<Training_NUM;i++){
		for(int j=0;j<Feature_DIM+2;j++)
			fscanf(training_file,"%lf",&dataset[i][j]);
	}
	for(int i=0;i<Training_NUM;i++){
		srand(time(NULL));
		int k=rand()%Training_NUM;
		memcpy(rand_data[i],dataset[k],sizeof(dataset[k]));
	}
	node* root=(node*)malloc(sizeof(node));
	plant(Training_NUM,rand_data,root);




	return 0;
}