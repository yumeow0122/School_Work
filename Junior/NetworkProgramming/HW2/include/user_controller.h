#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED

#define MAX_USER_ID 10
#define MAX_USER_NAME 10
#define MAX_USER_IP 20

typedef struct UserData
{
    int id;
    char *name;
    char *ip;
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

int get_min_id(User *head);
#endif // DOUBLELINK_H_INCLUDED
