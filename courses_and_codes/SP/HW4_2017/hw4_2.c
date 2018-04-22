//4_1: structureds
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
//double training_dataset[Training_NUM][Feature_DIM]={{}};
//int training_label[Feature_DIM];

typedef struct 
{
	int id;
	double feature[Feature_DIM];
	int label;
}datapoint;
datapoint dataset[Training_NUM];
datapoint rand_data[Training_NUM];

static int cmp(const void *a, const void *b,  void *D)
{
      int c = *(int *)a;
      int d = *(int *)b;
      int f = *(int *)D;
      if(dataset[c].feature[f] < dataset[d].feature[f])
      	{return -1;}               //傳回 -1 代表 a < b
      else if (dataset[c].feature[f] == dataset[d].feature[f]) 
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

void gini_impurity(int N, datapoint* data,node* now){
	int allone=1,allzero=1;
	double min_imp=2;
	int min_cut,min_f;	
	int sorted_id[N];
	int sorted_count[N];
	short skipped[N];
	int count; 
	for(int i=0;i<N;i++){
		if(data[i].label==1)	allzero=0;
		else					allone=0;
		sorted_id[i]=data[i].id;
	}
	for(int f=0;f<Feature_DIM;f++){
		qsort_r(sorted_id,N,sizeof(int),cmp,&f);
		count = 0;
		for(int i=0;i<N;i++){
			if(data[sorted_id[i]].label==0) count++;
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
	now->right = NULL;
	now->left = NULL;
	if(allone || allzero){
		now->isleaf = 1;
		if(allzero) now->label=0;
		else		now->label=1;
	}
	else{
		now->isleaf=0;
		now->label=-1;
	}
	return;
}

int main(int argc,char** argv){
	FILE* training_file = fopen("training_data","r");
	int id;
	memset(dataset,0,sizeof(dataset));
	for(int i=0;i<Training_NUM;i++){
		fscanf(training_file,"%d",&dataset[i].id);
		for(int j=0;j<Feature_DIM;j++)
			fscanf(training_file,"%lf",&dataset[i].feature[j]);
		fscanf(training_file,"%d",&dataset[i].label);
	}
	for(int i=0;i<Training_NUM;i++){
		srand(time(NULL));
		int k=rand()%Training_NUM;
		memcpy(rand_data[i],dataset[k],sizeof(datapoint));
	}
	node* root;
	gini_impurity(Training_NUM,rand_data,root);




	return 0;
}