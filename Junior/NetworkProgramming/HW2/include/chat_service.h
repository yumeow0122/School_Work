#ifndef CHAT_SERVICE_H_INCLUDED
#define CHAT_SERVICE_H_INCLUDED

#include "user_controller.h"
#include "command_controller.h"

void who(User *uhead, User *user, int socketFD);

#endif // CHAT_SERVICE_H_INCLUDED
