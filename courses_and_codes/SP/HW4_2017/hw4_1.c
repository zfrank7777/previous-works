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
const int Feature_DIM = 33;
const int Training_NUM = 25150;
//double training_dataset[Training_NUM][Feature_DIM]={{}};
//int training_label[Feature_DIM];

typedef struct 
{
	int id;
	double feature[Feature_DIM];
	int label;
}datapoint;
datapoint dataset[Training_NUM];

static int cmp(const void *a, const void *b, const void *D)
{
      int c = *(int *)a;
      int d = *(int *)b;
      int f = *(int *)D;
      if(dataset[c].feature[D] < dataset[d].feature[D])
      	{return -1;}               //傳回 -1 代表 a < b
      else if (dataset[c].feature[D] == dataset[d].feature[D]) {return 0;}      //傳回   0 代表 a = b
      else return 1;                          //傳回  1 代表 a>b
}


void gini_impurity(int N, datapoint* data){
	double min_imp[Feature_DIM]={};
	int min_cut[Feature_DIM]={};	
	int sorted_id[N];
	int sorted_count[N];
	short skipped[N];
	double tmpmax;
	int tmpid, count; 
	for(int f=0;f<Feature_DIM;f++){
		//sort f-th feature (n^2)
		count=0;
		for(int i=0;i<N;i++){
			sorted_count[i]=0;
			skipped[i]=0;
		} 
		int skip;
		for(int i=N-1;i>=0;i--){
			//printf("%d\n",i );
			tmpmax=-1000000;
			tmpid=-1;
			skip=-1;
			for(int j=0;j<=i;j++)
				if(data[j].feature[f] > tmpmax && skipped[j]==0){
					//printf("kk\n");
					tmpid = data[j].id; 
					//printf("find id:%d (%d)\n",tmpid,i );
					tmpmax = data[j].feature[f];
					skip = j;
				}
			if(data[tmpid].label==1) count++;
			sorted_id[N-i] = tmpid;
			sorted_count[N-i] = count;
			skipped[skip] = 1;
			printf("%d %d %lf\n", N-i,tmpid,);
		}
		for(int i=0;i<5;i++)
			printf("%d %d %lf %d\n",f,sorted_id[N-i], data[sorted_id[N-i]].feature[f], sorted_count[N-i]);
		//calculate the min impurity
/*		min_imp[f]=2;
		double imp, denom, numer;
		for(int i=0;i<N-1;i++){
			imp = 0;
			//upper half
			denom = (i+1);
			numer = sorted_count[i];
			imp += (numer/denom)*(1-(numer/denom));
			printf("%lf %lf\n", numer,denom);
			//lower half
			denom = (N-i-1);
			numer = (count-sorted_count[i]);
			printf("	%lf %lf\n", numer,denom);
			imp += (numer/denom)*(1-(numer/denom));
			if(imp < min_imp[f]){
				min_imp[f] = imp;
				min_cut[f] = i; //cut below i
			}
		}
		printf("\\%lf, %d\n",min_imp[f],min_cut[f] );
*/
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
	gini_impurity(Training_NUM,dataset);

	return 0;
}