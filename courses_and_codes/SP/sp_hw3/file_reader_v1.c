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
#define MAXBUFSIZE  10000
/*
typedef struct {
    char c_time_string[100];
} TimeInfo;
*/
int main(int argc, char** argv) 
{
 //   fprintf(stderr, "XD\n");
    sleep(10);
    int fd;
    char* filename = argv[1];
    fd = open(filename,O_RDWR);
    if(fd==-1){
        char errmsg[8]="exit(1)";
        write(STDOUT_FILENO,errmsg,8);
        exit(1);
    }
    void* mmapRead = mmap(0,MAXBUFSIZE,PROT_READ,MAP_SHARED,fd,0);
 //   fprintf(stderr, "end mmap\n");
    close(fd);
    write(STDOUT_FILENO,mmapRead,strlen(mmapRead));
 //   fprintf(stderr, "end write\n");
    //fprintf(stderr,"check mmap read:%d\n", STDOUT_FILENO);
//    printf("after mmap:%s\n",mmapRead);
/*    
    int fd,i;
    time_t current_time;
    char c_time_string[100];
    TimeInfo *p_map;
    
    const char  *file ="time_test";

    fd = open(file, O_RDWR);
    p_map = (TimeInfo*)mmap(0, sizeof(TimeInfo),  PROT_READ,  MAP_SHARED, fd, 0);
    
    while(1) {
        sleep(2);
        printf("%s\n", p_map->c_time_string);
    }
*/
    return 0;
}