#include "../include/authen.h"

enum LogoutState logout (int connfd, char *user_id, char *pass, Session sessions[], int max_sessions, User users[], int max_users) {
    Session *ss = findSessionByConnfd(connfd, sessions, max_sessions);
    if (ss == NULL) return -1;
    else if (ss->user != NULL && ss->user->state == BLOCKED) {
        // remove user from session
        removeUserFromSession(ss);

        return LO_USER_BLOCKED;
    }
    else if (ss->state == NOT_IDENTIFIED_USER) return LO_NOT_IDENTIFIED_USER;

    User *user = findUserById(user_id, users, max_users);
    // UserID not found or wrong user
    if (user == NULL || user != ss->user) return LO_WRONG_USER;

    if (ss->state == NOT_AUTHENTICATED     // Defined user but not AUTHENTICATED
        || !strcmp(user->pass, pass)) {       // Logout successful
        removeUserFromSession(ss);
        
        return LO_SUCCESS;
    } else {    // Wrong password
        return LO_WRONG_PASS;
    }
}

enum LoginPassState loginPass (int connfd, char *pass, Session sessions[], int max_sessions, User users[], int max_users) {
    Session *ss = findSessionByConnfd(connfd, sessions, max_sessions);
    if (ss == NULL) return -1;
    else if (ss->user != NULL && ss->user->state == BLOCKED) {
        // remove user from session
        removeUserFromSession(ss);

        return LP_USER_BLOCKED;
    }
    else if (ss->state == NOT_IDENTIFIED_USER) return LP_NOT_IDENTIFIED_USER;
    else if (ss->state == AUTHENTICATED) return LP_EXISTED_ONE_USER;

    // Defined user
    if (!strcmp(ss->user->pass, pass)) {    // Login success
        ss->state = AUTHENTICATED;
        ss->no_login_fail = 0;

        return LP_SUCCESS;
    } else {    // Login fail
        ss->no_login_fail++;

        if (ss->no_login_fail >= MAX_NO_LOGIN_FAIL) {    // Login fail too much
            // block user
            ss->user->state = BLOCKED;
            // remove user from session
            removeUserFromSession(ss);

            return LP_WRONG_PASS_TOO_MUCH;
        } else {
            return LP_WRONG_PASS;
        }
    }
}

enum LoginUserState loginUser (int connfd, char *user_id, Session sessions[], int max_sessions, User users[], int max_users) {
    Session *ss = findSessionByConnfd(connfd, sessions, max_sessions);
    if (ss == NULL || ss->state == USER_BLOCKED)
        return -1;
    else if (ss->state == NOT_AUTHENTICATED || ss->state == AUTHENTICATED)
        return LU_EXISTED_ONE_USER;

    User *user = findUserById(user_id, users, max_users);
    if (user == NULL) {
        printf("User NOTFOUND\n");
        return LU_NOT_FOUND;
    } else if (user->state == BLOCKED) return LU_USER_BLOCKED;
    else {
        updateSessionUser(ss->id, user, sessions, max_sessions);

        return LU_SUCCESS;
    }
}