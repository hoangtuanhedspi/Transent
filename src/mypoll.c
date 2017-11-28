#include "../include/mypoll.h"

_Bool removePoll (struct pollfd *poll) {
    if (poll == NULL) return 0;

    poll->fd = -1;
    return 1;
}

_Bool addPoll(int connfd, short events) {
    int i = 0;
    for (int i = 0; i < POLLS; i++) {
        if (polls[i].fd == -1) {
            polls[i].fd = connfd;
            polls[i].events = events;
            return 1;
        }
    }
    return 0;
}

void initPolls () {
    for (int i = 0; i < POLLS; i++) {
        polls[i].fd = -1;
    }
}