#ifndef _TS_FILE_MANAGE_
#define _TS_FILE_MANAGE_

#include "../include/session.h"
#include "../include/util.h"
#include "../include/slist.h"

typedef struct _file_cache{
    char* file_name;
    Session* session;
}TSFileCache;

typedef Node CacheList;

int init_cache_context();

extern TSFileCache * new_file_cache(Session* session,char* file_name);

extern int add_file_to_cache(CacheList** list,TSFileCache *file_cache);

#endif