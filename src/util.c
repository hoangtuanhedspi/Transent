#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "../include/util.h"

int parse_packet(char* buff,char* payload,int* size){
    int req_method = UNDEFINE;
    req_method = extract_request(buff);
	bzero(payload,PAY_LEN);
    *size = detach_payload2(buff,payload);
    return req_method;
}

int wrap_packet(char* buff,char* payload,int size,int method){
    add_request(buff,method);
    attach_payload(buff,payload,size);
    return size+HEADER_LEN+MD_LEN;
}

Var enqueue(Queue** queue,Var req_file){
    return append(queue,req_file)->data;
}

Var dequeue(Queue** queue){
    Node * res = get_at(*queue,0);
    Var data = clone_data(res);
    remove_node(queue,res);
    return data;
}

Var pop(Queue* queue){
    if(!queue) return NULL;
    Node * res = NULL;
    res = get_at(queue,0);
    if(!res) return NULL;
    return res->data;
}

char* clone_string(char* src){
    if(!src) return NULL;
    int len = strlen(src);
    char* res = tsalloc(char,len);
    if(!res) return NULL;
    bzero(res,len);
    strcpy(res,src);
    return res;
}

