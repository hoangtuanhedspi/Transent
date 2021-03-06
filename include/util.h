/*
 * Various utilities for transent system
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

#ifndef _TRANSENT_UTIL_
#define _TRANSENT_UTIL_
/**
 * @file
 * Utilities function for Transent
 */

#ifdef DEBUG
#include <errno.h>
//#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define logerr(M, ...) fprintf(stderr, "[ERROR - line:%d] in FILE[%s] at FUNC[%s]:" M "\n",\
                              __LINE__, __FILE__, __func__, /*clean_errno(),*/ ##__VA_ARGS__)
#define logwarn(M, ...) fprintf(stderr, "[WARN - line:%d] in FILE[%s] at FUNC[%s]:" M "\n",\
                              __LINE__, __FILE__, __func__, /*clean_errno(),*/ ##__VA_ARGS__)
#define loginfo(M, ...) fprintf(stderr, "[INFO - line:%d] in FILE[%s] at FUNC[%s]:" M "\n",\
                              __LINE__, __FILE__, __func__, /*clean_errno(),*/ ##__VA_ARGS__)
#define check(A, M, ...) if(!(A)) { logerr(M, ##__VA_ARGS__);}
#define check_mem(A) check(A, "Memory allocation error.")
#else
#define logerr(M, ...)
#define logwarn(M, ...)
#define loginfo(M, ...)
#define check(A, M, ...)
#define check_mem(A)
#endif
#include "slist.h"
#include "session.h"
#include "tsfmanage.h"
#include "interface.h"

#define MAX_SYNC_SIZE 24
#define ARGERR "Error: Too few arguments!\n"
#define PARGERR "Error: Invalid port argument!\n"
#define ADDERR "Error: Invalid address argument!\n"
#define tsalloc(type,size) (type*)malloc(size*sizeof(type))

typedef Node Queue;
/**
 * 
 */
extern int parse_packet(char* buff,char* payload,int* size);

/**
 * 
 */
extern int wrap_packet(char* buff,char* payload,int size,int method);

/**
 * 
 */
extern Var enqueue(Queue** queue,Var request);

/**
 * 
 */
extern Var dequeue(Queue** queue);

/**
 * 
 */
Var pop(Queue* queue);


/**
 * 
 */
extern char* clone_string(char* src);

#endif