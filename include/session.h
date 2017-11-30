#ifndef SESSION_H
#define SESSION_H

#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>

#define SESSIONS 5

typedef struct Session_ {
    struct sockaddr_in *cliaddr;
    int connfd;
} Session;

Session *findSessionByConnfd (Session* sessions,int connfd);
_Bool newSession (Session* sessions,struct sockaddr_in *cliaddr, int connfd);
_Bool removeSession (Session* sessions,Session *ss);
_Bool isSameSession (Session *ss1, Session *ss2);
void initSessions (Session* sessions);
_Bool isEqualSockAddrIn (struct sockaddr_in *addr1, struct sockaddr_in *addr2);
extern Session* copy_session(Session* session);
extern int clone_session(Session *des,Session* res);
#endif