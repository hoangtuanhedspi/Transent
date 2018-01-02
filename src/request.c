#include "../include/request.h"

int compare_request(Var des,Var res){
    //Todo: Fix compare session for duplicate file name
    return strcmp(((Request*)des)->file_name,((Request*)res)->file_name) == 0;
}

int drop_request(Queue** queue,Request* req){
    int res = remove_data(queue,compare_request,req);
    free(req);
    return res;
}

Request* new_request_object(Session* session, char * file_name, int timeout){
    Request* res = tsalloc(Request,sizeof(Request));
    res->session = copy_session(session);
    res->timeout = timeout;
    res->file_name = tsalloc(char,CFN_LEN);
    bzero(res->file_name,CFN_LEN);
    strcpy(res->file_name,file_name);
    return res;
}

Request* make_request(Session* session,char* file_name, int timeout){
    Request* res = NULL;
    res = tsalloc(Request,1);
    res->session = copy_session(session);
    res->file_name = clone_string(file_name);
    res->timeout = timeout;
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