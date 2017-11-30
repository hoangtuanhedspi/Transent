#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <assert.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define DEBUG 1
#include <transent/util.h>
#include <transent/interface.h>
#include <transent/mypoll.h>
#include <transent/directory.h>
#include <transent/session.h>
#include <transent/tsfmanage.h>
#include <transent/slist.h>
typedef Node CacheList;
void test_interface(int argc, char* argv[]);
void test_poll(int argc, char* argv[]);
void test_directory(int argc, char* argv[]);
void test_session(int argc, char* argv[]);
void test_tsfmanage(int argc, char* argv[]);

int main(int argc, char* argv[]){
    test_directory(argc,argv);
    test_interface(argc,argv);
    test_poll(argc,argv);
    test_session(argc,argv);
    test_tsfmanage(argc,argv);
    return 0;
}

void test_directory(int argc, char* argv[]){
    printf("==============TEST DIRECTORY===============\n");
    if(existFile("tests/","test.c"))
        printListFile("./tests/");
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
    loginfo("\nPackage contain %d byte\nmethod:%dpayload:%s\npayload_size:%dbyte\n",get_real_len(buff),i,pl,get_payload_size(buff));
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

void test_session(int argc, char* argv[]){
    printf("==============TEST SESSION===============\n");
    struct sockaddr_in myaddr;
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(3490);
    inet_aton("63.161.169.137", &myaddr.sin_addr.s_addr);
    printf("pass!\n");
    Session sessions[SESSIONS];
    initSessions(sessions);
    newSession(sessions,&myaddr,1);
    printf("pass!\n");
    Session* copy = copy_session(sessions);
    sessions[0].connfd = 2;
    inet_aton("63.161.169.126", &myaddr.sin_addr.s_addr);
    assert(copy->connfd == 1);
    printf("Copy client: %s sockfd:%d\n",inet_ntoa(copy->cliaddr->sin_addr),copy->connfd);
    Session* clone = tsalloc(Session,1);
    clone_session(clone,sessions);
    assert(clone->connfd == 2);
    printf("Clone client: %s sockfd:%d\n",inet_ntoa(clone->cliaddr->sin_addr),clone->connfd);
}

void test_tsfmanage(int argc, char* argv[]){
    printf("==============TEST TSFMANAGE===============\n");
    CacheList* cacheList = NULL;
    TSFileCache *cache =  new_cache_file(NULL,"abc");

    if(cache->session==NULL){
        printf("Run!\n");
    }

    append(&cacheList,cache);
    TSFileCache* ca = get_data(cacheList);
    assert(strcmp(ca->file_name,"abc")==0);
    printf("File name:%s\n",ca->file_name);
}