#include "../include/ss.h"

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

    return 1;
}

// typedef struct Session_ {
//     char id[ID_LEN];
//     User *user;
//     enum SessionState state;
//     struct sockaddr_in *cliaddr;
//     int connfd;
//     int no_login_fail;
// } Session;

_Bool removeSession (char *sessionId, Session sessions[], int max_sessions) {
    int target = indexOfSession(sessionId, sessions, max_sessions);
    if (target == -1) {     // Not found session
        printf("WARNING: Not found session %s\n", sessionId);
        return 0;
    }

    /* Remove session */
    sessions[target].id[0] = '\0';

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

    return 1;
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
    }
}

int id = 0;
char *hashId () {
    char *str = (char *) malloc(ID_LEN * sizeof(char));
    sprintf(str, "%d", id++);
    return str;
}