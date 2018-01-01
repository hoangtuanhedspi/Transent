#ifndef _TS_FILE_MANAGE_
#define _TS_FILE_MANAGE_
#include "slist.h"
#include "interface.h"
#define CFN_LEN 94
#define UID_HASH_LEN ((BUFF_SIZE-HEADER_LEN)/24 - CFN_LEN)

typedef struct _cache{
    char file_name[CFN_LEN];
    char uid_hash[UID_HASH_LEN];
}Cache;

typedef Node CacheList;

/**
 * Init cache file list manager context
 * @return -1 if finish, else return 1
 */
extern int init_cache_context(CacheList** cache_list);

/**
 * Create new cache file object
 * @param file_name name Point to string save founded file name from 
 * server
 * @param file_name point to hash guid of client connect to server contain
 * founded file
 * @return NULL if add failed else return point to new \ref Cache
 * object
 */
extern Cache * new_cache(char* file_name,char* uid_hash);

/**
 * Add new cache object to cache list in cache context
 * @param cache_list Point to list of cache
 * @param file_cache Represent an cache file object
 * @return negative value if add failed else return 1
 */
extern Cache* push_cache(CacheList * list,Cache* cache);

extern int get_cache_size(CacheList *list);

extern int get_all_cache_size();

extern int remove_cache(CacheList * list,Cache * cache);

extern int cache_contain(CacheList * list,Cache * cache);

extern Cache* list_cache_to_array(CacheList* list);

#endif