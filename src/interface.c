#include "../include/interface.h"

int add_request(char* buff,int method){
    bzero(buff,BUFF_SIZE);
    return memcpy(buff,&method,HEADER_LEN);
}

char* attach_payload(char* buff,char* payload){
    bzero(buff+HEADER_LEN,BUFF_SIZE-HEADER_LEN);
    return strcpy(buff+HEADER_LEN,payload);
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