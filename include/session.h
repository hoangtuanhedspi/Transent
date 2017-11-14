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

Session *findSessionByConnfd (int connfd);
_Bool newSession (struct sockaddr_in *cliaddr, int connfd);
_Bool removeSession (Session *ss);
_Bool isSameSession (Session *ss1, Session *ss2);
void initSessions ();
_Bool isEqualSockAddrIn (struct sockaddr_in *addr1, struct sockaddr_in *addr2);

#endif