//4_3: building tree
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
#define Tree_Size 20
//double training_dataset[Training_NUM][Feature_DIM]={{}};
//int training_label[Feature_DIM];

typedef struct 
{
	int id;
	double feature[Feature_DIM];
	int label;
}datapoint;
datapoint dataset[Training_NUM];
datapoint rand_data[Tree_Size];

static int cmp(const void *a, const void *b,  void *D){
      datapoint c = *(datapoint *)a;
      datapoint d = *(datapoint *)b;
      int f = *(int *)D;
      if(c.feature[f] < d.feature[f])
      	{return -1;}               //傳回 -1 代表 a < b
      else if (c.feature[f] == d.feature[f]) 
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

void plant(int N, datapoint* data,node* now){
	//if(N<=2) printf("almost leaf\n");
	int total_one=0;
	double min_imp=2;
	int min_cut,min_f;	
	int sorted_count[N];
	int count; 
	for(int i=0;i<N;i++)
		if(data[i].label==1) total_one++;
	//printf("%d\n",total_one );
	//end condition
	if(total_one==0 || total_one==N){
		now->isleaf=1;
		return;
	} 	
	//sort & calculate min impurity
	int upper=0;
	int lower=total_one;
	for(int f=0;f<Feature_DIM;f++){
		qsort_r(data,N,sizeof(datapoint),cmp,&f);
		count = 0;
		for(int i=0;i<N;i++){
			if(data[i].label==0) count++;
			sorted_count[i]=count;
		}
		double imp, denom, numer;
		for(int i=0;i<N-1;i++){
			imp = 0;
			denom = (i+1);
			numer = sorted_count[i];
			imp += (numer/denom)*(1-(numer/denom));
			denom = (N-i-1);
			numer = (count-sorted_count[i]);
			imp += (numer/denom)*(1-(numer/denom));
			if(imp < min_imp){
				min_imp = imp;
				min_cut = i;
				min_f = f;
			}
		}
	}
	printf("%d %lf %d\n",min_f,min_imp,min_cut);	
	now->feature = min_f;
	now->impurity = min_imp;
	now->cut = min_cut;
	now->right = (node*)malloc(sizeof(node));
	now->left = (node*)malloc(sizeof(node));
	now->isleaf = 0;
	//build children
	qsort_r(data,N,sizeof(datapoint),cmp,&min_f);
	plant(min_cut+1,rand_data,now->left);
	plant(N-min_cut-1,&rand_data[min_cut+1],now->right);
	return;
}

void traverse(node* now){
	if(now->left->isleaf==0)traverse(now->left);
	printf("%d,%lf, %d\n",now->feature,now->impurity,now->cut );
	if(now->right->isleaf==0)traverse(now->right);	
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
	srand(time(NULL));
	for(int i=0;i<Tree_Size;i++){
		int k=rand()%Training_NUM;
		rand_data[i] = dataset[k];
		//printf("%d %d %d\n",k, dataset[k].feature[1],dataset[k].label);
	}
	node* root=(node*)malloc(sizeof(node));
	plant(Training_NUM,rand_data,root);
	traverse(root);
	return 0;
}