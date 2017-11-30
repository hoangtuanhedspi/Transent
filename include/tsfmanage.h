#ifndef _TS_FILE_MANAGE_
#define _TS_FILE_MANAGE_

typedef struct _Node Node;
typedef struct Session_ Session;

typedef struct _file_cache{
    char* file_name;
    Session* session;
}TSFileCache;
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
extern TSFileCache * new_cache_file(Session* session,char* file_name);

extern int add_file_to_cache(Node** list,TSFileCache *file_cache);

#endif