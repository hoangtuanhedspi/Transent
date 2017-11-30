#include <string.h>
#include "../include/session.h"
#include "../include/util.h"

Session *findSessionByConnfd (Session* sessions,int connfd) {
    int i;
    for (i = 0; i < SESSIONS; i++) {
        if (sessions[i].connfd == connfd) {
            return sessions + i;
        }
    }

    return NULL;
}

_Bool newSession (Session* sessions,struct sockaddr_in *cliaddr, int connfd) {
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

_Bool removeSession (Session* sessions,Session *ss) {
    for (int i = 0; i < SESSIONS; i++) {
        if (isSameSession(sessions + i, ss)) {
            sessions[i].cliaddr = NULL;
            sessions[i].connfd = -1;
            
            return 1;
        }
    }
    
    return 0;
}

_Bool isSameSession(Session *ss1, Session *ss2) {
    if (ss1 == NULL || ss2 == NULL) {
        return 0;
    } else if (isEqualSockAddrIn(ss1->cliaddr, ss2->cliaddr) && ss1->connfd == ss2->connfd)
        return 1;
    else
        return 0; 
}

void initSessions(Session* sessions) {
    for (int i = 0; i < SESSIONS; i++) {
        sessions[i].cliaddr = NULL;
        sessions[i].connfd = -1;
    }
}

_Bool isEqualSockAddrIn (struct sockaddr_in *addr1, struct sockaddr_in *addr2) {
    return (addr1->sin_addr.s_addr == addr2->sin_addr.s_addr && addr1->sin_port == addr2->sin_port);
}

Session* copy_session(Session* session){
    Session *copy = tsalloc(Session,1);
    if(!copy) return NULL;
    if(clone_session(copy,session)==-1)
        return NULL;
    return copy;
}

int clone_session(Session *des,Session* res){
    if(!des || !res) return -1;
    
    des->connfd = res->connfd;

    if(!res->cliaddr){
        des->cliaddr = res->cliaddr;
        return 1;
    }
    
    if(!des->cliaddr)
        des->cliaddr = tsalloc(struct sockaddr_in,1);
    if(!des->cliaddr) return -1;
    
    memcpy(des->cliaddr,res->cliaddr,
           sizeof(struct sockaddr_in));
    return 1;
}
