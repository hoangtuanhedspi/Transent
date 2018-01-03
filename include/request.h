#ifndef _TRANSENT_REQUEST_
#define _TRANSENT_REQUEST_

#define DEFAULT_TIMEOUT 30

#include "util.h"
#include "session.h"

typedef struct _request{
    //Request session
    Session *session;
    //Request file name
    char* file_name;
    //Timeout in milisecond
    int timeout;
}Request;

/**
 * 
 */
extern Request* make_request(Session* session,char* file_name, int timeout);

/**
 * 
 */
extern int get_list_request(CacheList* list,Cache* cache,char* file_name);

/**
 * 
 */
extern int drop_request(Queue** queue,Request* req);

/**
 * 
 */
extern Request* new_request_object(Session* session, char * file_name, int timeout);

#endif