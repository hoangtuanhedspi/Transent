#include "../include/authen.h"

enum LoginUserState loginUser (int connfd, char *user_id, Session sessions[], int max_sessions, User users[], int max_users) {
    Session *ss = findSessionByConnfd(connfd, sessions, max_sessions);
    if (ss == NULL || ss->state == USER_BLOCKED)
        return -1;
    else if (ss->state == NOT_AUTHENTICATED || ss->state == AUTHENTICATED)
        return LU_EXISTED_ONE_USER;

    User *user = findUserById(user_id, users, max_users);
    if (user == NULL) return LU_NOT_FOUND;
    else if (user->state == BLOCKED) return LU_USER_BLOCKED;
    else {
        ss->user = user;
        ss->state = NOT_AUTHENTICATED;

        return LU_SUCCESS;
    }
}