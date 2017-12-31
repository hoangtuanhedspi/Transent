#include <string.h>
#include <stdlib.h>
#include "../include/command.h"
#include "../include/interface.h"

void strupp(char* src)
{
    while (*src = toupper(*src))
        src++;
}

Command* parse_cmd(char* args){
    Command* cmd = (Command*)malloc(sizeof(Command));
    bzero(cmd,sizeof(Command));
    strcpy(cmd->str_cmd,args);
    sscanf(args,"%[^ ]%*[^0-9a-z]%[^\n ]",cmd->method,cmd->data);
    strupp(cmd->method);
    return cmd;
}

int valid_cmd(Command cmd){
    Method m = stom(cmd.method);
    if(m != UNDEFINE)
    return m;
    return UNDEFINE;    
}

Command* suggest_cmd(Command cmd){
    return NULL;
}