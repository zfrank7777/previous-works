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
    int fd,fd2,i;
    time_t current_time;
    char c_time_string[100];
    Info *p_map;
    const char  *file =argv[2];
    fd = open(file, O_RDWR | O_TRUNC | O_CREAT, 0777); 
    if(fd<0)
    {
        perror("open");
        exit(-1);
    }
    lseek(fd,sizeof(Info),SEEK_SET);
    write(fd,"",1);
    p_map = (Info*) mmap(0, sizeof(Info), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    current_time = time(NULL);
    strcpy(c_time_string, ctime(&current_time));
    char buf[200];
    memcpy(p_map->lastfile_name, argv[1],strlen(argv[1]));
    memcpy(p_map->c_time_string, &c_time_string , sizeof(c_time_string));
    munmap(p_map, sizeof(Info));

    return 0;
}