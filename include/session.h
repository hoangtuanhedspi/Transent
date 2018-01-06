#ifndef SESSION_H
#define SESSION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "user.h"
#include "util.h"

#define ID_LEN 10
#define MAX_NO_LOGIN_FAIL 5

enum SessionState {
    NOT_IDENTIFIED_USER,    // Undefine user
    NOT_AUTHENTICATED,      // Defined user, not password
    AUTHENTICATED,          // Authenticated
    USER_BLOCKED            // User is blocked
};

typedef struct Session_ {
    char id[ID_LEN];
    User *user;
    enum SessionState state;
    struct sockaddr_in *cliaddr;
    int connfd;
    int no_login_fail;
} Session;

_Bool updateSessionState (char *sessionId, enum SessionState state, Session sessions[], int max_sessions);
_Bool updateSessionUser (char *sessionId, User *user, Session sessions[], int max_sessions);
_Bool removeSession (char *sessionId, Session sessions[], int max_sessions);
_Bool newSession (struct sockaddr_in *cliaddr, int connfd, Session sessions[], int max_sessions);
_Bool removeUserFromSession (Session *session);
Session *findSessionByConnfd (int connfd, Session sessions[], int max_sessions);
Session *findSessionById(char* id, Session sessions[], int max_sessions);
int indexOfSession (char *sessionId, Session sessions[], int max_sessions);
int indexOfEmptySession (Session sessions[], int max_sessions);
void initSessions (Session sessions[], int max_sessions);
int isSameSession(Session* des, Session* res);
char *hashId ();
int clone_session(des,res);
Session* copy_session(Session* session);
int session_size();
void printSessions ();

// _Bool isEqualSockAddrIn (struct sockaddr_in *addr1, struct sockaddr_in *addr2);
// _Bool isSameSession (struct sockaddr_in *addr1, int connfd1, struct sockaddr_in *addr2, int connfd2);

#endif
