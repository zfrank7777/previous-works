//4_10: add commandline arg
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
#include <pthread.h>
#define Feature_DIM 33
#define Training_NUM 25150
#define Tree_Size 200
#define Tree_Num 5000 
#define Testing_NUM 25008
#define Thread_NUM 5000

typedef struct 
{
	int id;
	double feature[Feature_DIM];
	int label;
}datapoint;
datapoint dataset[Training_NUM];
datapoint tree_data[Tree_Num][Tree_Size];
static int cmp(const void *a, const void *b,  void *D){
      datapoint c = *(datapoint *)a;
      datapoint d = *(datapoint *)b;
      int f = *(int *)D;
      if(c.feature[f] < d.feature[f])
      	{return -1;}                 //傳回 -1 代表 a < b
      else if (c.feature[f] == d.feature[f]) 
      	{return 0;}     			 //傳回   0 代表 a = b
      else return 1;                 //傳回  1 代表 a>b
}

typedef struct Node{
	int feature;
	double threshold;
	struct Node* left;
	struct Node* right;
	int isleaf;
	int label;
}node;

void plant(int N, datapoint* data,node* now,int depth){
	int total_one=0;
	double min_imp=2;
	int min_cut,min_f;	
	for(int i=0;i<N;i++)
		total_one+=data[i].label;
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
	imp = 2* ( (upper/(i+1))*(1-(upper/(i+1))) + (lower/(N-i-1))*(1-(lower/(N-i-1))) );
			if(imp < min_imp){
				min_imp = imp;
				min_cut = i;
				min_f = f;
			}
		}
	}
	qsort_r(data,N,sizeof(datapoint),cmp,&min_f);
	now->feature = min_f;
	now->threshold = data[min_cut].feature[min_f];
	now->right = (node*)malloc(sizeof(node));
	now->left = (node*)malloc(sizeof(node));
	now->isleaf = 0;
	//build children
	plant(min_cut+1,data,now->left,depth+1);
	plant(N-min_cut-1,&data[min_cut+1],now->right,depth+1);
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
pthread_t thread_id[Tree_Num];
node* root[Tree_Num];

void* thr_fn(void* arg){
	int treeid = arg;
	plant(Tree_Size,tree_data[treeid],root[treeid],1);
	//printf("done plant tree %d\n", treeid);
	pthread_exit((void*)0);
}

int main(int argc,char** argv){
	//manage argc,argv
	if(argc%2==0){
		fprintf(stderr, "invalid argument number:%d\n",argc );
		fprintf(stderr, "usage: ./hw4 -data data_dir -output output_file -tree tree_number -thread thread_number\n");
		return 0;
	}
	fprintf(stderr, "default: data_dir=. , output_file=./submission.csv, tree_number=200, thread_number=16\n");
	char data_dir[100]=".";
	char output_file[100]="./submission.csv";
	int tree_number=200,thread_number=16;
	for(int i=1;i<argc;i+=2){
		//fprintf(stderr, "%d %d\n",argc,i );
		if(strcmp(argv[i],"-data")==0) {
			memset(data_dir,0,sizeof(data_dir));
			strcpy(data_dir,argv[i+1]);
		}
		else if(strcmp(argv[i],"-output")==0) {
			memset(output_file,0,sizeof(output_file));
			strcpy(output_file,argv[i+1]);
		}
		else if(strcmp(argv[i],"-tree")==0){
			//fprintf(stderr, "treeeeee\n", ); 
			tree_number = atoi(argv[i+1]);
			if(tree_number > Tree_Num){
				fprintf(stderr, "error: MAX_TREE_NUM = 5000\n");
				return 0;
			} 
		}
		else if(strcmp(argv[i],"-thread")==0){
			thread_number = atoi(argv[i+1]);
			if(thread_number > Thread_NUM){
				fprintf(stderr, "error: MAX_THREAD_NUM = 5000\n");
				return 0;
			} 
		}
		else{
			fprintf(stderr, "invalid argument\n");
			fprintf(stderr, "usage: ./hw4 -data data_dir -output output_file -tree tree_number -thread thread_number\n");
			return 0;
		}
	}
	fprintf(stderr, "set: data_dir=%s , output_file=%s, tree_number=%d, thread_number=%d\n",data_dir,output_file,tree_number,thread_number );
	char training_filename[100];
	sprintf(training_filename,"%s/training_data",data_dir);
	FILE* training_file = fopen(training_filename,"r");
	memset(dataset,0,sizeof(dataset));
	for(int i=0;i<Training_NUM;i++){
		fscanf(training_file,"%d",&dataset[i].id);
		for(int j=0;j<Feature_DIM;j++)
			fscanf(training_file,"%lf",&dataset[i].feature[j]);
		fscanf(training_file,"%d",&dataset[i].label);
	}
	srand(time(NULL));
	for(int x=0;x<tree_number;x++)	
		root[x]=(node*)malloc(sizeof(node));

	int y;
	for(y=0;y<thread_number;y++){
		if(y>=tree_number) break;
		for(int i=0;i<Tree_Size;i++){
			int k=rand()%Training_NUM;
			tree_data[y][i] = dataset[k];
		}		
		pthread_create(&thread_id[y],NULL,thr_fn,y);
	}
	printf("done create all threads\n");
	while(y<tree_number)
		for(int x=0;x<thread_number;x++)
			if((pthread_tryjoin_np(thread_id[x],NULL))==0){
				for(int i=0;i<Tree_Size;i++){
					int k=rand()%Training_NUM;
					tree_data[y][i] = dataset[k];
				}
				pthread_create(&thread_id[x],NULL,thr_fn,y);
				y++;						
				break;
			}
	for(y=0;y<thread_number;y++){
		if(y>=tree_number) break;
		pthread_join(thread_id[y],NULL);
	}
	printf("done constructing trees\n");
	char testing_filename[100];
	sprintf(testing_filename,"%s/testing_data",data_dir);
	FILE* testing_file = fopen(testing_filename,"r");
	FILE* answer_file = fopen(output_file,"w+");
	fprintf(answer_file, "id,label\n");
	datapoint test;
	for(int i=0;i<Testing_NUM;i++){
		fscanf(testing_file,"%d",&test.id);
		for(int j=0;j<Feature_DIM;j++)
			fscanf(testing_file,"%lf",&test.feature[j]);
		int yes=0;
		for(int j=0;j<tree_number;j++)
			yes+=vote(test,root[j]);
		int result;
		if(yes>(tree_number/2)) result=1;
		else	result=0;
		fprintf(answer_file,"%d,%d\n",i,result);
	}
	printf("done voting\n");

	return 0;
}