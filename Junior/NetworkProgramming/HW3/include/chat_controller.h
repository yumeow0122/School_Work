#ifndef CHAT_CONTROLLER_H_INCLUDED
#define CHAT_CONTROLLER_H_INCLUDED

#include "user_controller.h"

typedef struct ChatArgs
{
    int socketFD;
    User *user;
    User *uhead;
} ChatArgs;

void *chat_client(void *arg);
void send_msg(int fd, char *msg);
void recv_msg(int fd, char *msg);
#endif // CHAT_CONTROLLER_H_INCLUDED
