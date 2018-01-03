#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <assert.h>
#include <string.h>
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
#include <transent/command.h>
#include <transent/request.h>

#define USERS 100
#define SESSIONS 100
#define USER_FILE "tests/user.txt"

void test_interface(int argc, char* argv[]);
void test_poll(int argc, char* argv[]);
void test_directory(int argc, char* argv[]);
void test_session(int argc, char* argv[]);
void test_tsfmanage(int argc, char* argv[]);
void test_haihv(int argc, char* argv[]);
void test_command(int argc, char* argv[]);

int main(int argc, char* argv[]){
    //test_directory(argc,argv);
    //test_interface(argc,argv);
    //test_poll(argc,argv);
    //test_session(argc,argv);
    test_tsfmanage(argc,argv);
    //test_haihv(argc,argv);
    //test_command(argc,argv);
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
    add_request(buff,RP_STREAM);
    int i = extract_request(buff);
    assert(i==RP_STREAM);
    attach_payload(buff,"Hello",5);
    add_meta_data(buff,"test.pdf");
    packet_info(buff);
    assert(strcmp(get_meta_data(buff),"test.pdf")==0);
    char* pl = detach_payload(buff);
    int j = 0;
    for(j = 0;j<16;j++){
        printf("decode:%d\n",buff[j]);
    }
    loginfo("\nPackage contain %d byte\nmethod:%d\nmetadata:%s\npayload:%s\npayload_size:%dbyte\n",get_real_len(buff),i,get_meta_data(buff),pl,get_payload_size(buff));
    assert(strcmp(pl,"Hello")==0);
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
    Session sessions[SESSIONS];
    initSessions(sessions,SESSIONS);
    newSession(&myaddr, 4, sessions, SESSIONS);
    printf("pass!\n");
    Session* copy = copy_session(sessions);
    sessions[0].connfd = 2;
    inet_aton("63.161.169.126", &myaddr.sin_addr.s_addr);
    assert(copy->connfd == 4);
    printf("Copy client: %s sockfd:%d\n",inet_ntoa(copy->cliaddr->sin_addr),copy->connfd);
    Session* clone = tsalloc(Session,1);
    clone_session(clone,sessions);
    assert(clone->connfd == 2);
    printf("Clone client: %s sockfd:%d\n",inet_ntoa(clone->cliaddr->sin_addr),clone->connfd);
}

void test_tsfmanage(int argc, char* argv[]){
    CacheList* list = NULL;
    printf("Sizeof one cache:%d\n",sizeof(Cache));
    init_cache_context(&list);
    Queue* queue = NULL;
    Request *r = make_request(NULL,"abc",10);
    enqueue(&queue,make_request(NULL,"abc",10));
    enqueue(&queue,make_request(NULL,"def",10));
    if(is_requested(queue,r)){
        printf("Requested\n");
    }
    printf("LOL:%s\n",((Request*)queue->data)->file_name);
    Cache* cache = new_cache("abc","def");
    assert(strcmp(cache->file_name,"abc")==0);
    
    push_cache(list,cache);
    remove_cache(list,cache);
    cache = new_cache("abc","def");
    push_cache(list,cache);
    cache = new_cache("abg","duf");
    push_cache(list,cache);
    Cache* get = (Cache*)(get_at(list,1)->data);

    assert(strcmp(get->file_name,"abg")==0);
    if(cache_contain(list,get))
        printf("Cache Size:%d|In all:%d\n",get_cache_size(list),get_all_cache_size());
    Cache req[24];
    printf("Pass!\n");
    int s = get_list_request(list,req,"abg");
    int i = 0;
    for (i=0;i<s;i++)
        printf("Request file:%s\n",req[i].uid_hash);
    
    //drop_request(&queue,make_request(NULL,"abc",10));
    Cache* arr = list_cache_to_array(list);
    for(int i = 0; i< get_cache_size(list);i++){
        printf("%s-%s\n",arr[i].file_name,arr[i].uid_hash);
    }
    printf("Queue size:%d\n",length(queue));
}

void test_haihv(int argc, char* argv[]){
    printf("==============TEST HAIHV===============\n");
    User users[USERS];
    Session sessions[SESSIONS];
    initSessions(sessions, SESSIONS);


    readUsers(USER_FILE, users, USERS);
    printUsers(users, USERS);
    
    struct sockaddr_in *sock = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    newSession(sock, 4, sessions, SESSIONS);
    newSession(sock, 4, sessions, SESSIONS);
    newSession(sock, 6, sessions, SESSIONS);
    newSession(sock, 7, sessions, SESSIONS);


    loginUser(4, "haihv", sessions, SESSIONS, users, USERS);
    //loginUser(5, "admin", sessions, SESSIONS, users, USERS);
    loginUser(6, "notfound", sessions, SESSIONS, users, USERS);
    loginUser(7, "block", sessions, SESSIONS, users, USERS);

    loginPass(4, "wrong", sessions, SESSIONS, users, USERS);
    loginPass(4, "wrong", sessions, SESSIONS, users, USERS);
    loginPass(4, "wrong", sessions, SESSIONS, users, USERS);
    loginPass(4, "wrong", sessions, SESSIONS, users, USERS);
    // loginPass(4, "haihv", sessions, SESSIONS, users, USERS);
    //loginPass(5, "admin", sessions, SESSIONS, users, USERS);
    loginPass(6, "notfound", sessions, SESSIONS, users, USERS);
    loginPass(7, "block", sessions, SESSIONS, users, USERS);

    logout(4, "haihv", "wrong", sessions, SESSIONS, users, USERS);
    //logout(5, "admin", "admin", sessions, SESSIONS, users, USERS);
    logout(6, "notfound", "notfound", sessions, SESSIONS, users, USERS);
    logout(7, "block", "block", sessions, SESSIONS, users, USERS);
    printf("Size:%d\n",session_size());

    printSessions(sessions, SESSIONS);
    //assert(isSameSession()==1)
}

void test_command(int argc, char* argv[]){
    printf("============Test command==============\n");
    char* command[10] = {"logIn        hoangtuan *\n",
                         "PASS        124567&\%aHS\n",
                         "FIND test.pdf\n"};
    
    for(int i = 0; i< 3; i++){
        Command * cmd = parse_cmd(command[i]);
        if(valid_cmd(*cmd))
            printf("Command:%smethod:%d\ndata:%s\n",cmd->str_cmd,stom(cmd->method),cmd->data);
    }
    //assert(RP_FOUND == RQ_FILE);
    char* hello = "Hello";
    Var* data = hello;
    printf("Sizeof:%d\n",sizeof(data));
}