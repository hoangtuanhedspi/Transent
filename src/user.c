#include "../include/user.h"

User *createUser (char *id, char *pass) {
    User *user = (User *) malloc(sizeof(User));

    strncpy(user->id, id, USER_ID_LEN);
    strncpy(user->pass, pass, PASS_LEN);
    user->state = ACTIVE;

    return user;
}

enum NewUserState newUser (User *user, User users[], int max_users) {
    if (findUserById(user->id, users, max_users) != NULL) {
        // Existed user
        return EXISTED;
    } else {
        for (int i = 0; i < max_users; i++) {
            if (users[i].id[0] == '\0') {
                strcpy(users[i].id, user->id);
                strcpy(users[i].pass, user->pass);
                users[i].state = user->state;

                free(user);
                return SUCCESS;
            }
        }

        return FULL;
    }
}

User *findUserById (char *id, User users[], int max_users) {
    for (int i = 0; i < max_users; i++) {
        if (!strcmp(users[i].id, id))       // Found user
            return (users + i);
    }

    return NULL;
}

int indexOfUser (char *id, User users[], int max_users) {
    for (int i = 0; i < max_users; i++) {
        if (!strcmp(users[i].id, id))       // Found user
            return i;
    }

    return -1;
}

void writeUsers (char *file_name, User users[], int max_users) {
    FILE *f = openFile(file_name, "w");

    /* Store users from array users to file */
    for (int i = 0; i < max_users; i++) {
        if (users[i].id[0] != '\0') {
            fprintf(f, "%s %s %d\n", users[i].id, users[i].pass, users[i].state);
        }
    }

    fclose(f);
}

void readUsers (char *file_name, User users[], int max_users) {
    FILE *f = openFile(file_name, "r");

    initUsers(users, max_users);

    /* Store users from file to array users */
    int i = 0;
    while (i < max_users) {
        if (feof(f)) break;

        fscanf(f, "%s %s %d", users[i].id, users[i].pass, &(users[i].state));

        // Ignore blank last line of file
        if (users[i].id[0] == '\0' || users[i].pass[0] == '\0') break;
        
        i++;
    }

    fclose(f);
}

_Bool isFullUsers (User users[], int max_users) {
    for (int i = 0; i < max_users; i++) {
        if (users[i].id[0] == '\0') {       // User has id != ""
            return 0;
        }
    }

    return 1;
}

void initUsers (User users[], int max_users) {
    for (int i = 0; i < max_users; i++) {
        users[i].id[0] = '\0';
    }
}

void printUsers (User users[], int max_users) {
    for (int i = 0; i < max_users; i++) {
        if (users[i].id[0] != '\0')
            printf("%s | %s | %d\n", users[i].id, users[i].pass, users[i].state);
    }
}

FILE *openFile (char *file_name, char *mode) {
    FILE *f = fopen(file_name, mode);
    if (f == NULL) {
        perror("ERROR_OPEN_FILE");
        exit(EXIT_FAILURE);
    }

    return f;
}