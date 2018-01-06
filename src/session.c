#include <string.h>
#include "../include/session.h"

static int size = 0;
_Bool updateSessionState (char *sessionId, enum SessionState state, Session sessions[], int max_sessions) {
    int target = indexOfSession(sessionId, sessions, max_sessions);
    if (target == -1) {     // Not found session
        printf("WARNING: Not found session %s\n", sessionId);
        return 0;
    }

    /* Update session */
    sessions[target].state = state;

    return 1;
}

_Bool updateSessionUser (char *sessionId, User *user, Session sessions[], int max_sessions) {
    int target = indexOfSession(sessionId, sessions, max_sessions);
    if (target == -1) {     // Not found session
        printf("WARNING: Not found session %s\n", sessionId);
        return 0;
    }

    /* Update session */
    if (user == NULL) {
        printf("WARNING: Update session's user fail. User pointer is null\n");
        return 0;
    }
    sessions[target].user = user;
    sessions[target].state = NOT_AUTHENTICATED;

    return 1;
}

_Bool removeUserFromSession (Session *session) {
    if (session == NULL) {     // Not found session
        printf("WARNING: Not found session\n");
        return 0;
    }

    /* Remove user from session */
    session->user = NULL;
    session->state = NOT_IDENTIFIED_USER;
    session->no_login_fail = 0;

    return 1;
}

_Bool removeSession (char *sessionId, Session sessions[], int max_sessions) {
    int target = indexOfSession(sessionId, sessions, max_sessions);
    if (target == -1) {     // Not found session
        printf("WARNING: Not found session %s\n", sessionId);
        return 0;
    }

    /* Remove session */
    sessions[target].id[0] = '\0';
    sessions[target].cliaddr = NULL;
    sessions[target].connfd = -1;

    size--;

    return 1;
}

Session* copy_session(Session* session){
    Session *copy = tsalloc(Session, 1);

    if(!copy) return NULL;

    if(clone_session(copy,session) == -1)
        return NULL;
        
    return copy;
}

int clone_session(des,res)
Session *des;
Session *res;
{
    if(!des || !res) return -1;
    des->connfd = res->connfd;
    des->state = res->state;
    des->no_login_fail = res->no_login_fail;
    strcpy(des->id,res->id);

    if(!res->user)
        des->user = res->user;

    if(!res->cliaddr){
        des->cliaddr = res->cliaddr;
        return 1;
    }

    if(!des->user)
        des->user = tsalloc(User,1);
    
    if(!des->user) return -1;

    if(!des->cliaddr)
        des->cliaddr = tsalloc(struct sockaddr_in,1);

    if(!des->cliaddr) return -1;

    if(res->user!=NULL)
        memcpy(des->user,res->user,sizeof(User));
    
    if(res->cliaddr!=NULL)
        memcpy(des->cliaddr,res->cliaddr,sizeof(struct sockaddr_in));
    
    return 1;
}

_Bool newSession (struct sockaddr_in *cliaddr, int connfd, Session sessions[], int max_sessions) {
    int emptySession = indexOfEmptySession(sessions, max_sessions);
    if (emptySession == -1) {       // Full sessions
        printf("ERROR: Full sessions!\n");
        return 0;
    }

    /* Init new session */
    char *hash_id = hashId();
    strcpy(sessions[emptySession].id, hash_id);
    free(hash_id);
    sessions[emptySession].user = NULL;
    sessions[emptySession].state = NOT_IDENTIFIED_USER;
    sessions[emptySession].cliaddr = cliaddr;
    sessions[emptySession].connfd = connfd;
    sessions[emptySession].no_login_fail = 0;

    size++;

    return 1;
}

Session *findSessionByConnfd (int connfd, Session sessions[], int max_sessions) {
    for (int i = 0; i < max_sessions; i++) {
        if (sessions[i].connfd != -1 && sessions[i].connfd == connfd)
            return (sessions + i);
    }

    return NULL;
}

Session *findSessionById(char* id, Session sessions[], int max_sessions) {
    for (int i = 0; i < max_sessions; i++) {
        if (sessions[i].connfd != -1 && strcmp(sessions[i].id,id) == 0)
            return (sessions + i);
    }
    return NULL;
}

int indexOfSession (char *sessionId, Session sessions[], int max_sessions) {
    for (int i = 0; i < max_sessions; i++) {
        if (sessions[i].id[0] != '\0' && !strcmp(sessions[i].id, sessionId)) {
            return i;       // Found session
        }
    }

    return -1;      // Not found sessions
}

int indexOfEmptySession (Session sessions[], int max_sessions) {
    for (int i = 0; i < max_sessions; i++) {
        if (sessions[i].id[0] == '\0') {
            return i;       // Found empty session
        }
    }

    return -1;      // Full sessions
}

void initSessions (Session sessions[], int max_sessions) {
    for (int i = 0; i < max_sessions; i++) {
        sessions[i].id[0] = '\0';
        sessions[i].user = NULL;
        sessions[i].cliaddr = NULL;
        sessions[i].connfd = -1;
    }
}

int id = 0;
char *hashId () {
    char *str = (char *) malloc(ID_LEN * sizeof(char));
    sprintf(str, "%d", id++);
    return str;
}

void printSessions (Session sessions[], int max_sessions) {
    for (int i = 0; i < max_sessions; i++) {
        if (sessions[i].id[0] != '\0') {
            printf("%s | %d | %d | %d | %d | %d\n", sessions[i].id, sessions[i].user, sessions[i].state, sessions[i].cliaddr, sessions[i].connfd, sessions[i].no_login_fail);
        }
    }
}

int isSameSession(Session* des, Session* res){
    if(!des || !res) return 0;
    return strcmp(des->id, res->id) == 0;
}


int session_size(){
    return size;
}
