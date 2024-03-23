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
int recv_msg(char *msg);
#endif // CHAT_CONTROLLER_H_INCLUDED
