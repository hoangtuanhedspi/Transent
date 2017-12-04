#include <stdio.h>
#include <stdlib.h>
#include "../include/user.h"

#define USERS 100
#define USER_FILE "tests/user.txt"

int main(int argc, char* argv[]){
    User users[USERS];

    readUsers(USER_FILE, users, USERS);
    int i = indexOfUser("haihv", users, USERS);
    printf("%d\n", i);

    for (int i = 0; i < USERS; i++) {
        if (users[i].id[0] != '\0') {
            printf("%s | %s | %d\n", users[i].id, users[i].pass, users[i].status);
        }
    }

    return 0;
}