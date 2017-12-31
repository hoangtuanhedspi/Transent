#include <string.h>
#include "../include/command.h"

Command* parse_cmd(char* args){
    Command* cmd = (Command*)malloc(sizeof(Command));
    bzero(cmd,sizeof(Command));
    strcpy(cmd->str_cmd,args);
    sscanf(args,"%[^ ]%*[^0-9a-z]%[^\n ]",cmd->method,cmd->data);
    return cmd;
}