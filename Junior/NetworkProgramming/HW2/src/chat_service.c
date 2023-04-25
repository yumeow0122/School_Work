#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chat_service.h"
#include "chat_controller.h"
#include "user_controller.h"
#include "command_controller.h"
void who(User *uhead, User *user, int socketFD)
{
    int idx = user->data->id;
    char *tmp = malloc(MAX_OUTPUT_SIZE * sizeof(char *));
    char *msg = malloc(MAX_OUTPUT_SIZE * sizeof(char *));

    const char *TID = "<ID>";
    const char *TNAME = "<NAME>";
    const char *TIP = "<IP>";

    snprintf(tmp, MAX_OUTPUT_SIZE, "%4s %10s %20s\n", TID, TNAME, TIP);
    strcpy(msg, tmp);
    User *cur = uhead->next;
    while (cur != uhead)
    {
        UserData *udata = cur->data;
        snprintf(tmp, MAX_OUTPUT_SIZE, "%4d %10s %20s", udata->id, udata->name, udata->ip);
        strcat(msg, tmp);
        if (idx == udata->id)
        {
            snprintf(tmp, MAX_OUTPUT_SIZE, "    <-");
            strcat(msg, tmp);
        }
        snprintf(tmp, MAX_OUTPUT_SIZE, "\n");
        strcat(msg, tmp);
        cur = cur->next;
    }
    send_msg(socketFD, msg);
}

void name(User *uhead, User *user, int socketFD, char *newName)
{
    char *msg = malloc(MAX_OUTPUT_SIZE * sizeof(char *));
    if (can_change_user_name(uhead, user, newName))
    {
        strcpy(user->data->name, newName);
        snprintf(msg, MAX_OUTPUT_SIZE, "Change user name sucess.\n");
    }
    else
    {
        snprintf(msg, MAX_OUTPUT_SIZE, "User name alreaady exist.\n");
    }
    send_msg(socketFD, msg);
}
