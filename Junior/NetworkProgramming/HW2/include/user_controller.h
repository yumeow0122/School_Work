#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED

#define MAX_USER_ID 10
#define MAX_USER_NAME 10
#define MAX_USER_IP 20

#include "pipe_controller.h"
typedef struct UserData
{
    int id;
    char *name;
    char *ip;

    Pipe *pipeHead;
} UserData;

typedef struct User
{
    UserData *data;
    struct User *prev;
    struct User *next;
} User;

UserData *user_data_init();
User *user_init();

void log_all_user(User *head, int idx);
void add_user(User *head, User *nuser);
void delete_user(User *user);
void change_user_name(User *head, User *user, char *name);
int get_min_id(User *head);

char *get_ip_port(char *ip, char *port);
#endif // DOUBLELINK_H_INCLUDED
