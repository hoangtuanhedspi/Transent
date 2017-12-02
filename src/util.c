#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "../include/interface.h"
#include "../include/util.h"

MenuAction get_user_method(int logedin){
    int ret = -1;
    printf("1.Send file\n2.Logout\nSelect:");
    scanf("%d",&ret);
    while(getchar()!='\n');
    if(ret==1) ret = SENDFILE;
    else if(ret==2) ret = LOGOUT;
    else ret = UNKNOWN;
    return valid_action(ret);
}

MenuAction valid_action(MenuAction action){
    switch(action){
        case SENDFILE:
        case LOGIN:
        case LOGOUT: return action;
        default: return UNKNOWN;
    }
    return UNKNOWN;
}

int parse_packet(char* buff,char* payload,int* size){
    int req_method = UNDEFINE;
    req_method = extract_request(buff);
	bzero(payload,PAY_LEN);
    *size = detach_payload2(buff,payload);
    return req_method;
}