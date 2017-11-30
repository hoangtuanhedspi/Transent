#include "../include/tsfmanage.h"

int init_cache_context(){
    return -1;
}

TSFileCache * new_file_cache(Session* session,char* file_name){
    TSFileCache* cache = tsalloc(TSFileCache,1);
    if(!cache)
        return NULL;
    cache->file_name = file_name;
    cache->session = copy_session(session);
    return cache;
}

int add_file_to_cache(CacheList** list,TSFileCache *file_cache){
    if(append(list,file_cache)!=NULL) 
        return 1;
    return -1;
}