/* mmap_write.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    char c_time_string[100];    
    char lastfile_name[100];
} Info;

int main(int argc, char** argv) 
{
    //char* after_header;
    //scanf("%s",after_header);
    int fd,i,fd2;
    time_t current_time;
    char c_time_string[100];
    Info *p_map;    
    const char  *file =argv[1];
    fd = open(file, O_RDWR);
    p_map = (Info*)mmap(0, sizeof(Info),  PROT_READ,  MAP_SHARED, fd, 0);
    printf("Last read time: %sLast read file: %s" , p_map->c_time_string,p_map->lastfile_name);
    /*
    while(1) {
        sleep(2);
        printf("%s%s\n", p_map->c_time_string,name_map);
    }
    */
    return 0;
}