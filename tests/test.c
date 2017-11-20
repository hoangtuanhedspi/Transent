#include <stdio.h>
#include <stdlib.h>
#include <transent/directory.h>

#define A 0x0001
#define B 0x0000 >> 1

int main(int argc, char* argv[]){
    int a[6] = {1,3,4,5,8,7},b = 0;
    for (b = 0;a[b]!=10,b<5;b++);
    printf("%d:%d\n",a[b],b);
    printListFile("/Users/anonymousjp/Desktop/");
    return 0;
}