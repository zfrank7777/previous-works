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
double training_dataset[Training_NUM][Feature_DIM]={{}};
int training_label[Feature_DIM];
typedef struct 
{
	int id;
	double feature[Feature_DIM];
}datapoint;

int gini_impurity(){
	double min_imp[Feature_DIM]={};
	int min_cut[Feature_DIM]={};	

	int sorted_id[Training_NUM];
	double tmpmax;
	int tmpid; 
	for(int f=0;f<Feature_DIM;f++){
		//sort f-th feature (n^2)
		for(int i=Training_NUM-1;i>=0;i++){
			tmpmax=training_dataset[0][f];
			tmpid=1;
			for(int j=0;j<i;j++)
				if(training_dataset[j][f] > tmpmax){
					tmpid= j;
					tmpmax = training_dataset[j][f];
				}
			sorted_id[i] = tmpid;
		}
		//calculate the min impurity
		min_imp[f]=2;
		double imp;
		int denom, numer;
		for(int i=1;i<Training_NUM;i++){
			imp=0;
			denom=i;
			numer=0;
			for(int j=0;j<i;j++)
				if(training_label[sorted_id[j]]==1)numer++;
			imp += (numer/denom)*(1-(numer/denom));
			denom=Training_NUM-i;
			numer=0;
			for(int j=i;j<Training_NUM;j++) 
				if(training_label[sorted_id[j]]==1)numer++;
			imp += (numer/denom)*(1-(numer/denom));
			if(imp < min_imp[f]){
				min_imp[f] = imp;
				min_cut[f] = i;
			}
		}
	}

}
int main(int argc,char** argv){
	FILE* training_file = fopen("training_data","r");
	int id;
	for(int i=0;i<Training_NUM;i++){
		fscanf(training_file,"%d",&id);
		for(int j=0;j<Feature_DIM;j++)
			fscanf(training_file,"%lf",&training_dataset[i][j]);
		fscanf(training_file,"%d",&training_label[i]);

	}
	int min = gini_impurity();

	return 0;
}