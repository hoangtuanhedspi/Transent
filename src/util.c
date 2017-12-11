#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "../include/interface.h"
#include "../include/util.h"
#include "../include/slist.h"
#include "../include/session.h"

MenuAction get_user_method(int logedin){
    int ret = -1;
    printf("1.Send file\n2.Logout\nSelect:");
    scanf("%d",&ret);
    while(getchar()!='\n');
    if(ret==1) ret = SENDFILE;
    else if(ret==2) ret = LOGOUT;
    else ret = UNKNOWN;
    return valid_action(ret);
}

MenuAction valid_action(MenuAction action){
    switch(action){
        case SENDFILE:
        case LOGIN:
        case LOGOUT: return action;
        default: return UNKNOWN;
    }
    return UNKNOWN;
}

int parse_packet(char* buff,char* payload,int* size){
    int req_method = UNDEFINE;
    req_method = extract_request(buff);
	bzero(payload,PAY_LEN);
    *size = detach_payload2(buff,payload);
    return req_method;
}

int wrap_packet(char* buff,char* payload,int size,int method){
    add_request(buff,method);
    attach_payload(buff,payload,size);
    return size+HEADER_LEN;
}

Request* enqueue(Queue** queue,Request * req_file){
    return append(queue,req_file)->data;
}

Request* dequeue(Queue** queue){
    Node * res = get_at(*queue,0);
    remove_node(queue,res);
    return get_data(res);
}

int compare_queue(Var des,Var res){
    //Todo: Fix compare session for duplicate file name
    return strcmp(((Request*)des)->file_name,((Request*)res)->file_name) == 0;
}

int drop_request(Queue** queue,Request* req){
    int res = remove_data(queue,compare_queue,req);
    free(req);
    return res;
}

Request* new_request_object(Session* session, char * file_name){
    Request* res = tsalloc(Request,sizeof(Request));
    res->session = copy_session(session);
    res->file_name = tsalloc(char,CFN_LEN);
    bzero(res->file_name,CFN_LEN);
    strcpy(res->file_name,file_name);
    return res;
}

char* clone_string(char* src){
    if(!src) return NULL;
    int len = strlen(src);
    char* res = tsalloc(char,len);
    if(!res) return NULL;
    bzero(res,len);
    strcpy(res,src);
    return res;
}

Request* make_request(Session* session,char* file_name){
    Request* res = NULL;
    res = tsalloc(Request,1);
    res->session = copy_session(session);
    res->file_name = clone_string(file_name);
    return res;
}

int compare_name(Cache* des,char* fname){
    return strcmp(des->file_name,fname) == 0;
}

int get_list_request(CacheList* list,Cache* cache,char* file_name){
    int i = 0;
    if(!list || !cache) return 0;
    CacheList* tmp = list;
    while(i<MAX_SYNC_SIZE && tmp!=NULL){
        if(compare_name(tmp->data,file_name) == 1){
            cache[i] = *((Cache*)tmp->data);
            ++i;
        }
        tmp = tmp->next;
    }
    return i;
}