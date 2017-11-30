#include "../include/tsfmanage.h"

int init_cache_context(){
    return -1;
}

TSFileCache * new_file_cache(Session* session,char* file_name){
    return NULL;
}

int add_file_to_cache(CacheList** list,TSFileCache *file_cache){
    if(append(list,file_cache)!=NULL) 
        return 1;
    return -1;
}