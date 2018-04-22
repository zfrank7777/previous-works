//4_6: building tree
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
#define Tree_Size 100
#define Tree_Num 200
#define Testing_NUM 25008
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
	double threshold;
	double impurity;
	int cut;
	struct Node* left;
	struct Node* right;
	int isleaf;
	int label;
}node;

void plant(int N, datapoint* data,node* now,int depth){
	//if(N<=2) printf("almost leaf\n");
	int total_one=0;
	double min_imp=2;
	int min_cut,min_f;	
	for(int i=0;i<N;i++){
		total_one+=data[i].label;
		//printf("%d,%d\n",data[i].label,total_one );
	}
	//printf("%d\n",total_one );
	//end condition
	if(total_one==0 || total_one==N){
		now->isleaf=1;
		if(total_one==0) now->label=0;
		else now->label=1;
		return;
	} 	
	//sort & calculate min impurity
	double upper,lower;
	for(int f=0;f<Feature_DIM;f++){
		qsort_r(data,N,sizeof(datapoint),cmp,&f);
		double imp, denom, numer;
		upper=0; lower=total_one;
		for(int i=0;i<N-1;i++){
			if(data[i].label==1) {upper++;lower--;}
			imp = 2* ( (upper/(i+1))*(1-(upper/(i+1))) + 
						(lower/(N-i-1))*(1-(lower/(N-i-1))) );
			//printf("%lf %d %d\n",imp,upper,lower );
			if(imp < min_imp){
				min_imp = imp;
				min_cut = i;
				min_f = f;
			}
		}
	}
	printf("%d,%d\n",min_cut,min_f);	
	qsort_r(data,N,sizeof(datapoint),cmp,&min_f);
	now->feature = min_f;
	now->threshold = data[min_cut].feature[min_f];
	now->impurity = min_imp;
	now->cut = min_cut;
	now->right = (node*)malloc(sizeof(node));
	now->left = (node*)malloc(sizeof(node));
	now->isleaf = 0;
	//build children
	plant(min_cut+1,data,now->left,depth+1);
	plant(N-min_cut-1,&data[min_cut+1],now->right,depth+1);
	return;
}

void traverse(node* now){
	if(now->left->isleaf==0)traverse(now->left);
	printf("%d,%lf, %d\n",now->feature,now->impurity,now->cut );
	if(now->right->isleaf==0)traverse(now->right);	
	return;
}

int vote(datapoint testdata,node* a){
	if(a->isleaf==1)
		return a->label;
	int ftr = a->feature;
	if(testdata.feature[ftr] < a->threshold)
		return vote(testdata,a->left);
	else
		return vote(testdata,a->right);
}

int main(int argc,char** argv){
	FILE* training_file = fopen("training_data","r");
	memset(dataset,0,sizeof(dataset));
	for(int i=0;i<Training_NUM;i++){
		fscanf(training_file,"%d",&dataset[i].id);
		for(int j=0;j<Feature_DIM;j++)
			fscanf(training_file,"%lf",&dataset[i].feature[j]);
		fscanf(training_file,"%d",&dataset[i].label);
	}
	srand(time(NULL));
	node* root[Tree_Num];
	for(int x=0;x<Tree_Num;x++)	
		root[x]=(node*)malloc(sizeof(node));
	for(int x=0;x<Tree_Num;x++){
		for(int i=0;i<Tree_Size;i++){
			int k=rand()%Training_NUM;
			rand_data[i] = dataset[k];
			//printf("%d %d\n",k, rand_data[i].label);
		}
		plant(Tree_Size,rand_data,root[x],1);
		printf("done plant tree %d\n", x);
	}
	//traverse(root);
	FILE* testing_file = fopen("testing_data","r");
	FILE* answer_file = fopen("myans.csv","w+");
	fprintf(answer_file, "id,label\n");
	datapoint test;
	for(int i=0;i<Testing_NUM;i++){
		fscanf(testing_file,"%d",&test.id);
		for(int j=0;j<Feature_DIM;j++)
			fscanf(testing_file,"%lf",&test.feature[j]);
		int yes=0;
		for(int j=0;j<Tree_Num;j++)
			yes+=vote(test,root[j]);
		int result;
		if(yes>(Tree_Num/2)) result=1;
		else	result=0;
		fprintf(answer_file,"%d,%d\n",i,result);
	}

	return 0;
}