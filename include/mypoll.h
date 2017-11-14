#ifndef MYPOLL_H
#define MYPOLL_H

#include <stdio.h>
#include <stdlib.h>
#include <poll.h>

#define POLLS 6

struct pollfd polls[POLLS];

_Bool removePoll (struct pollfd *poll);
_Bool addPoll (int connfd, short events);
void initPolls ();

#endif