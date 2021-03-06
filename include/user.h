#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define USER_ID_LEN 30
#define PASS_LEN 30

enum UserState { 
    BLOCKED,
    ACTIVE
};

enum NewUserState {
    SUCCESS,
    EXISTED,
    FULL
};

typedef struct User_ {
    char id[USER_ID_LEN + 1];
    char pass[PASS_LEN + 1];
    enum UserState state;
} User;

User *findUserById (char *id, User users[], int max_users);
int indexOfUser (char *id, User users[], int max_users);
void writeUsers (char *file_name, User users[], int max_users);
void readUsers (char *file_name, User users[], int max_users);
_Bool isFullUsers (User users[], int max_users);
void initUsers (User users[], int max_users);
User *createUser (char *id, char *pass);
enum NewUserState newUser (User *user, User users[], int max_users);
void printUsers (User users[], int max_users);
FILE *openFile (char *file_name, char *mode);

#endif
