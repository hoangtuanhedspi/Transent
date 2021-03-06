/*
 * Various interface for communication between client-server 
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

#include<stdlib.h>
#include<string.h>
#include "../include/interface.h"
#include "../include/util.h"

struct {
    Method method;
    char string[CMETHOD_LEN + 1];
} c_method[METHOD_COUNT] = {
    {STUP,"So stupid"},
    {SIGNUP,"SIGNUP"},
    {LOGIN,"LOGIN"},
    {LOGOUT,"LOGOUT"},
    {FIND,"FIND"},
    {SELECT,"SELECT"},
    {EXIT,"EXIT"}
};

int pack(int at);

void* add_request(char* buff,int method){
    bzero(buff+pack(1),PACK_SIZE);
    return memcpy(buff+pack(PACK_METHOD),&method,PACK_SIZE);
}

int pack(int at){
    at-=1;
    if(at<1) at = 0;
    return PACK_SIZE*at;
}

void* add_meta_data(char* buff, char* meta){
    bzero(buff + HEADER_LEN,MD_LEN);
    return memcpy(buff+HEADER_LEN,meta,MD_LEN);
}

char* get_meta_data(char* buff){
    char* meta = tsalloc(char,MD_LEN);
    bzero(meta,MD_LEN);
    memcpy(meta,buff+HEADER_LEN,MD_LEN);
    return meta;
}

int extract_request(char* buff){
    int req = UNDEFINE;
    memcpy(&req,buff+pack(PACK_METHOD),PACK_SIZE);
    return req;
}

int add_response_number(char* buff, int number){
    bzero(buff+pack(PACK_WIDEN),PACK_SIZE);
    memcpy(buff+pack(PACK_WIDEN),&number,PACK_SIZE);
    return number;
}

int extract_response_number(char* buff){
    int req = UNDEFINE;
    memcpy(&req,buff+pack(PACK_WIDEN),PACK_SIZE);
    return req;
}

void* attach_payload(char* buff,char* payload,unsigned int size){
    bzero(buff+HEADER_LEN+MD_LEN,PAY_LEN);
    memcpy(buff+pack(PACK_PAYSIZE),&size,PACK_SIZE);
    return memcpy(buff+HEADER_LEN+MD_LEN,payload,size);
}

char* detach_payload(char* buff){
    char* payload = NULL;
    payload = tsalloc(char,PAY_LEN);
    bzero(payload,PAY_LEN);
    memcpy(payload,buff+HEADER_LEN+MD_LEN,PAY_LEN);
    return payload;
}

int detach_payload2(char* buff,char* payload){
    bzero(payload,PAY_LEN);
    memcpy(payload,buff+HEADER_LEN+MD_LEN,PAY_LEN);
    return get_payload_size(buff);
}

int get_real_len(char* buff){
    int len = 0;
    len += HEADER_LEN + MD_LEN + get_payload_size(buff);
    return len;
}

int get_payload_size(char* buff){
    int size = 0;
    memcpy(&size,buff+pack(PACK_PAYSIZE),PACK_SIZE);
    return size;
}

int valid_method(int method){
    switch(method){
        case RQ_FILE:
        case RQ_FAIL:
        case RP_FOUND:
        case RP_NFOUND: return method;
        default: return UNDEFINE;
    }
    return UNDEFINE;
}

int stom(char* string_method){
    for(int i = 0;i<METHOD_COUNT;i++){
        if(strcmp(c_method[i].string, string_method)==0)
            return c_method[i].method;
    }
    return UNDEFINE;
}

char* mtos(Method method){
    return c_method[method].string;
}

void packet_info(char* buff){
    printf("\nPackage contain %d byte\n\
    method:%d\n\
    metadata:%s\n\
    payload:%s\n\
    payload_size:%dbyte\n",
    get_real_len(buff),
    extract_request(buff),
    get_meta_data(buff),
    detach_payload(buff),
    get_payload_size(buff));
}