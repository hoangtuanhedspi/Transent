/*
 * Various user function for transent system
 * copyright (c) 2017 Tuan Hoang
 *
 * This file is part of Transent.
 *
 * Transent is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * Transent is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Transent; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

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
void printUsers (User users[], int max_users);
FILE *openFile (char *file_name, char *mode);

#endif
