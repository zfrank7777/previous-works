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
#ifdef Sleep
    sleep(10);
#endif
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

    return 0;
}