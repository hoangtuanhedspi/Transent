#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <assert.h>
#include<string.h>
#define DEBUG 1
#include <transent/util.h>
#include <transent/interface.h>
#include <transent/mypoll.h>
#include <transent/directory.h>

void test_interface(int argc, char* argv[]);
void test_poll(int argc, char* argv[]);
void test_directory(int argc, char* argv[]);
int main(int argc, char* argv[]){
    test_directory(argc,argv);
    test_interface(argc,argv);
    test_poll(argc,argv);
    return 0;
}

void test_directory(int argc, char* argv[]){
    printf("==============TEST DIRECTORY===============\n");
    printListFile("./db");
}

void test_interface(int argc, char* argv[]){
    char buff[BUFF_SIZE];
    printf("==============TEST INTERFACE===============\n");
    add_request(buff,RQ_FILE);
    int i = extract_request(buff);
    attach_payload(buff,"Hello",3);
    char* pl = detach_payload(buff);
    int j = 0;
    for(j = 0;j<10;j++){
        printf("decode:%d\n",buff[j]);
    }
    loginfo("\nPackage contain %d byte\nmethod:%d at pack %d \npayload:%s\npayload_size:%dbyte\n",get_real_len(buff),i,pack(1),pl,get_payload_size(buff));
    assert(i==1);
    assert(strcmp(pl,"Hel")==0);
}

void test_poll(int argc, char* argv[]){
    printf("==============TEST POLL===============\n");
    initPolls();
    int i = 0;
    struct pollfd pol[1];
    printf("Add poll\n");

    for(i = 0; i <POLLS;i++)
        assert(addPoll(i,POLLIN)==1);

    for(i = 0; i <POLLS;i++){
        printf("Poll fd:%d\n",polls[i].fd);
    }

    printf("Remove poll at 0\n");
    assert(removePoll(polls));
    for(i = 0; i <POLLS;i++){
        printf("Poll fd:%d\n",polls[i].fd);
    }
}