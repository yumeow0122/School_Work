#ifndef SERVER_CONTROLLER_H_INCLUDED
#define SERVER_CONTROLLER_H_INCLUDED

#include "user_controller.h"

typedef struct ServerArgs
{
    int socketFD;
    User *user;
    User *uhead;
    redisContext *redis;
} ServerArgs;

void *server_client(void *arg);

#endif // SERVER_CONTROLLER_H_INCLUDED
