#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "user_controller.h"
#include "pipe_controller.h"

UserData *user_data_init()
{
    UserData *data = (UserData *)malloc(sizeof(UserData));
    data->id = 0;
    data->name = "no name";
    data->ip = malloc(MAX_USER_IP * sizeof(char *));
    data->pipeHead = pipe_init();
    return data;
}

User *user_init()
{
    User *user = (User *)malloc(sizeof(User));
    user->data = user_data_init();
    user->next = user;
    user->prev = user;
    return user;
}

void log_all_user(User *head, int idx)
{
    const char *TID = "<ID>";
    const char *TNAME = "<NAME>";
    const char *TIP = "<IP>";

    User *cur = head->next;
    printf("%4s %10s %20s\n", TID, TNAME, TIP);
    while (cur != head)
    {
        UserData *udata = cur->data;
        printf("%4d %10s %20s", udata->id, udata->name, udata->ip);
        if (idx == udata->id)
            printf("<-");
        printf("\n");
        cur = cur->next;
    }
}

void add_user(User *head, User *nuser)
{
    nuser->next = head;
    nuser->prev = head->prev;
    head->prev->next = nuser;
    head->prev = nuser;
}
void delete_user(User *user)
{
    user->prev->next = user->next;
    user->next->prev = user->prev;
    free(user);
}

int get_min_id(User *head)
{
    int record[MAX_USER_ID];
    memset(record, 0, MAX_USER_ID);
    int cnt = 0;
    User *cur = head->next;
    while (cur != head)
    {
        record[cur->data->id] = 1;
        cnt++;
        cur = cur->next;
    }
    record[cur->data->id] = 1;
    for (int idx = 0; idx < cnt; idx++)
    {
        if (!record[idx])
            return idx;
    }
    return cnt; // out of bound
}