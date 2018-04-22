#include <stdio.h>    // printf(), sprintf()

int main() {
    char str[10];
    int num = 961406;

    sprintf(str, "%d", num);
    printf("str = %s\n", str);

    return 0;
}