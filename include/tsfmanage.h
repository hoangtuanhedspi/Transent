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

/**
 * Init cache file list manager context
 * @return -1 if finish, else return 1
 */
extern int init_cache_context();

/**
 * Create new cache file object
 * @param session Point to session of client connect to server contain
 * founded file
 * @param file_name name Point to string save founded file name from server
 * @return NULL if add failed else return point to new \ref TSFileCache object
 */
extern TSFileCache * new_file_cache(Session* session,char* file_name);

extern int add_file_to_cache(CacheList** list,TSFileCache *file_cache);

#endif