#include "../include/user.h"

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
            fprintf(f, "%s %s %d\n", users[i].id, users[i].pass, users[i].status);
        }
    }

    fclose(f);
}

void readUsers (char *file_name, User users[], int max_users) {
    FILE *f = openFile(file_name, "r");

    /* Init/reset users[] */
    for (int i = 0; i < max_users; i++) {
        users[i].id[0] = '\0';
    }

    /* Store users from file to array users */
    int i = 0;
    while (i < max_users) {
        if (feof(f)) break;

        fscanf(f, "%s %s %d", users[i].id, users[i].pass, &(users[i].status));

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

FILE *openFile (char *file_name, char *mode) {
    FILE *f = fopen(file_name, mode);
    if (f == NULL) {
        perror("ERROR_OPEN_FILE");
        exit(EXIT_FAILURE);
    }

    return f;
}