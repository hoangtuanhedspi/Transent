#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <assert.h>
#include<string.h>
#define DEBUG 1
#include <transent/util.h>
#include <transent/interface.h>

void test_interface(int argc, char* argv[]);
int main(int argc, char* argv[]){
    test_interface(argc,argv);
    return 0;
}

void test_interface(int argc, char* argv[]){
    char buff[BUFF_SIZE];
    printf("==============Test interface===============\n");
    add_request(buff,1);
    int i = extract_request(buff);
    attach_payload(buff,"Hello",3);
    char* pl = detach_payload(buff);
    loginfo("\nPackage contain\nmethod:%d at pack %d \npayload:%s\npayload_size:%dbyte\n",i,pack(1),pl,get_payload_size(buff));
    assert(i==1);
    assert(strcmp(pl,"Hel")==0);
}