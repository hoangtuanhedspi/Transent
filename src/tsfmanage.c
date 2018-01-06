#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/util.h"
#include "../include/tsfmanage.h"

int is_init = 0;
int saved_cache_size = 0;

#define throw_init_error if(!is_init){\
    printf("Please initial cache context\
before using it!\n");\
    exit(0);\
}

int init_cache_context(CacheList** cache_list){
    if (!is_init)
    {
        is_init = 1;
        *cache_list = tsalloc(Node,1);
        (*cache_list)->data = NULL;
    }
    return 1;
}

Cache * new_cache(char* file_name,char* uid_hash){
    throw_init_error
    if(!file_name || !uid_hash) return NULL;
    Cache *res = tsalloc(Cache,1);
    if(!res) return NULL;
    strcpy(res->file_name,file_name);
    strcpy(res->uid_hash,uid_hash);
    res->life = CACHE_LIFE_TIME;
    return res;
}

Cache* push_cache(CacheList * list,Cache* cache){
    throw_init_error
    if(!list) return NULL;
    saved_cache_size ++;
    return n_append(list,cache)->data;
}

int get_cache_size(CacheList *list){
    throw_init_error
    if(!list) return 0;
    return length(list);
}

int get_all_cache_size(){
    return saved_cache_size;
}

int compare_cache(Var des,Var res){
    if(!des || !res) return 0;
    return strcmp(((Cache*)des)->file_name,((Cache*)res)->file_name) == 0 
           && strcmp(((Cache*)des)->uid_hash,((Cache*)res)->uid_hash) == 0;
}

int remove_cache(CacheList* list,Cache* cache){
    --saved_cache_size;
    if(list->next==NULL){
        list->data = NULL;
        return 0;
    }
    return remove_data(&list,compare_cache,cache);
}

int cache_contain(CacheList * list,Cache * cache){
    return find_data(list,compare_cache,cache) != NULL;
}

Cache* list_cache_to_array(CacheList* list){
    CacheList*  tmp = list;
    int         i   = 0;
	int length = get_cache_size(list);
	Cache* res = tsalloc(Cache,length);
	bzero(res,length*sizeof(Cache));
	while(tmp != NULL){
		res[i] = *((Cache*)tmp->data);
		tmp = tmp->next;i++;
	}
	return res;
}

LocalCache * new_local_cache(Cache* cache,int timeout){
    return NULL;
}
