#include <stdio.h>
#include <stdlib.h>
#include "transent/user.h"
#include "transent/ss.h"

#define USERS 100
#define SESSIONS 100
#define USER_FILE "tests/user.txt"

int main(int argc, char* argv[]){
    User users[USERS];
    Session sessions[SESSIONS];
    initSessions(sessions, SESSIONS);

    readUsers(USER_FILE, users, USERS);
    
    struct sockaddr_in *sock = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    newSession(sock, 4, sessions, SESSIONS);
    newSession(sock, 5, sessions, SESSIONS);
    newSession(sock, 6, sessions, SESSIONS);
    newSession(sock, 7, sessions, SESSIONS);
    
    updateSessionUser("0", users, sessions, SESSIONS);
    updateSessionUser("1", users + 1, sessions, SESSIONS);
    updateSessionUser("2", users + 2, sessions, SESSIONS);
    updateSessionUser("3", users + 3, sessions, SESSIONS);

    updateSessionState("0", NOT_AUTHENTICATED, sessions, SESSIONS);
    updateSessionState("1", AUTHENTICATED, sessions, SESSIONS);
    updateSessionState("2", USER_BLOCKED, sessions, SESSIONS);
    updateSessionState("3", NOT_IDENTIFIED_USER, sessions, SESSIONS);

    // removeSession("0", sessions, SESSIONS);
    removeSession("1", sessions, SESSIONS);
    // removeSession("2", sessions, SESSIONS);
    removeSession("3", sessions, SESSIONS);

    printSessions(sessions, SESSIONS);

    printf("%s\n", sessions[0].user->id);
    printf("%s\n", sessions[1].user->id);
    printf("%s\n", sessions[2].user->id);
    printf("%s\n", sessions[3].user->id);

    return 0;
}

// typedef struct Session_ {
//     char id[ID_LEN];
//     User *user;
//     enum SessionState state;
//     struct sockaddr_in *cliaddr;
//     int connfd;
//     int no_login_fail;
// } Session;