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