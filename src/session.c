#include "../include/session.h"
#include "../include/util.h"
Session *findSessionByConnfd (int connfd) {
    int i;
    for (i = 0; i < SESSIONS; i++) {
        if (sessions[i].connfd == connfd) {
            return sessions + i;
        }
    }

    return NULL;
}

_Bool newSession (struct sockaddr_in *cliaddr, int connfd) {
    int i;
    for (i = 0; i < SESSIONS; i++) {
        if (sessions[i].cliaddr == NULL) {
            sessions[i].cliaddr = cliaddr;
            sessions[i].connfd = connfd;

            return 1;
        }
    }
    return 0;
}

_Bool removeSession (Session *ss) {
    for (int i = 0; i < SESSIONS; i++) {
        if (isSameSession(sessions + i, ss)) {
            sessions[i].cliaddr = NULL;
            sessions[i].connfd = -1;
            
            return 1;
        }
    }
    
    return 0;
}

_Bool isSameSession (Session *ss1, Session *ss2) {
    if (ss1 == NULL || ss2 == NULL) {
        return 0;
    } else if (isEqualSockAddrIn(ss1->cliaddr, ss2->cliaddr) && ss1->connfd == ss2->connfd)
        return 1;
    else
        return 0; 
}

void initSessions () {
    for (int i = 0; i < SESSIONS; i++) {
        sessions[i].cliaddr = NULL;
        sessions[i].connfd = -1;
    }
}

_Bool isEqualSockAddrIn (struct sockaddr_in *addr1, struct sockaddr_in *addr2) {
    return (addr1->sin_addr.s_addr == addr2->sin_addr.s_addr && addr1->sin_port == addr2->sin_port);
}
