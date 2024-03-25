#ifndef CHAT_SERVICE_H_INCLUDED
#define CHAT_SERVICE_H_INCLUDED

#include "user_controller.h"
#include "command_controller.h"

void who(User *uhead, User *user, int socketFD);

void name(User *uhead, User *user, int socketFD, char *newName);

void tell(User *uhead, User *user, int socketFD, Command *cmd);

void yell(User *uhead, User *user, int socketFD, Command *cmd);

#endif // CHAT_SERVICE_H_INCLUDED
