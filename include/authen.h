#ifndef AUTHEN_H
#define AUTHEN_H

#include "user.h"
#include "session.h"

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

enum LogoutState logout (int connfd, char *user_id, char *pass, Session sessions[], int max_sessions, User users[], int max_users);
enum LoginPassState loginPass (int connfd, char *pass, Session sessions[], int max_sessions, User users[], int max_users);
enum LoginUserState loginUser (int connfd, char *user_id, Session sessions[], int max_sessions, User users[], int max_users);

#endif