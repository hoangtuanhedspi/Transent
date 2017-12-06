#ifndef SS_H
#define SS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include "user.h"

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

enum FunctionState {
    FN_SUCCESS,
    FN_ERROR
};

enum LoginUserState {
    LU_SUCCESS,                     // Login-user successful (define user)
    LU_EXISTED_ONE_USER,            // Existed a user in session
    LU_USER_BLOCKED,                // User is blocked
    LU_NOT_FOUND                    // User not found
};

enum LoginPassState {
    LP_SUCCESS,                     // Login successful
    LP_NOT_IDENTIFIED_USER,         // Undefine user
    LP_WRONG_PASS,                  // Wrong password
    LP_WRONG_PASS_TOO_MUCH,         // Wrong password too much
    LP_EXISTED_ONE_USER,            // Existed a user in session
    LP_USER_BLOCKED                 // User is blocked
};

enum LogoutState {
    LO_SUCCESS,                     // Logout successful
    LO_NOT_IDENTIFIED_USER,         // Undefine user
    LO_WRONG_USER,                  // Wrong user
    LO_WRONG_PASS,                  // Wrong password
    LO_USER_BLOCKED                 // User is blocked
};

_Bool updateSessionState (char *sessionId, enum SessionState state, Session sessions[], int max_sessions);
_Bool updateSessionUser (char *sessionId, User *user, Session sessions[], int max_sessions);
_Bool removeSession (char *sessionId, Session sessions[], int max_sessions);
_Bool newSession (struct sockaddr_in *cliaddr, int connfd, Session sessions[], int max_sessions);
int indexOfSession (char *sessionId, Session sessions[], int max_sessions);
int indexOfEmptySession (Session sessions[], int max_sessions);
void initSessions (Session sessions[], int max_sessions);
char *hashId ();

// _Bool addSession (Session *session, Session sessions[], int max_sessions);
// int indexOfSession (struct sockaddr_in *cliaddr, int connfd);
// _Bool isEqualSockAddrIn (struct sockaddr_in *addr1, struct sockaddr_in *addr2);

// enum LogoutState logout (struct sockaddr_in *cliaddr, int connfd, char *id, char *pass);
// enum LoginPassState loginPass (struct sockaddr_in *cliaddr, int connfd, char *pass);
// enum LoginUserState loginUser (struct sockaddr_in *cliaddr, int connfd, char *id);
// enum SessionState sessionState (struct sockaddr_in *cliaddr, int connfd);
// Session *findByConnfd (int connfd);
// Session *findBySessionCliaddr (struct sockaddr_in *cliaddr);
// Session *findSession (struct sockaddr_in *cliaddr, int connfd);
// void copySession (Session *src, Session *dest);
// _Bool isFullNoSession (Session sessions[], int max_sessions);
// _Bool isSameSession (struct sockaddr_in *addr1, int connfd1, struct sockaddr_in *addr2, int connfd2);
// void printSessions ();



#endif
