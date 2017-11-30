#include "../include/session.h"
#include "../include/util.h"
#include "../include/slist.h"
#include "../include/tsfmanage.h"

int init_cache_context(){
    return -1;
}

TSFileCache * new_cache_file(Session* session,char* file_name){
    TSFileCache* cache = tsalloc(TSFileCache,1);
    cache->session = NULL;
    if(!cache)
        return NULL;
    cache->file_name = file_name;
    printf("Pass!\n");
    cache->session = copy_session(session);
    return cache;
}

int add_file_to_cache(Node** list,TSFileCache *file_cache){
    printf("Pass!\n");
    if(append(list,file_cache)!=NULL) 
        return 1;
    return -1;
}