#ifndef _TRANSENT_COMMAND_
#define _TRANSENT_COMMAND_

#define METH_LEN 10
#define DATA_LEN 200

typedef struct _cmd{
    char str_cmd[METH_LEN+DATA_LEN];
    char method[METH_LEN];
    char data[DATA_LEN];
}Command;

int valid_str_cmd(char* args);
Command* parse_cmd(char* args);
int valid_cmd(Command cmd);
Command* suggest_cmd(Command cmd);
#endif