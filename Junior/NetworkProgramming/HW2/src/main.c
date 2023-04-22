#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "user_controller.h"

int main()
{
    printf("?\n");
    User *userHead = user_init();

    User *user1 = user_init();
    user1->data->id = get_min_id(userHead);
    add_user(userHead, user1);
    log_all_user(userHead, 0);

    User *user2 = user_init();
    user2->data->id = get_min_id(userHead);
    add_user(userHead, user2);
    log_all_user(userHead, 1);

    User *user3 = user_init();
    user3->data->id = get_min_id(userHead);
    add_user(userHead, user3);
    log_all_user(userHead, 2);

    delete_user(user2);
    log_all_user(userHead, 0);

    User *user4 = user_init();
    user4->data->id = get_min_id(userHead);
    add_user(userHead, user4);
    log_all_user(userHead, 2);


    User *user5 = user_init();
    user5->data->id = get_min_id(userHead);
    add_user(userHead, user5);
    log_all_user(userHead, 2);
    return 0;
}
