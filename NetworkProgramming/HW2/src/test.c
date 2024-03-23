#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user_controller.h"

// int main()
// {
//     User *head = user_init();

//     User *u0 = user_init();
//     u0->data->id = get_min_id(head);
//     add_user(head, u0);

//     log_all_user(head, 0);

//     change_user_name(head, u0, "user a");
//     log_all_user(head, 0);

//     User *u1 = user_init();
//     u1->data->id = get_min_id(head);
//     add_user(head, u1);

//     log_all_user(head, 1);
//     change_user_name(head, u1, "user a");
//     log_all_user(head, 1);
//     change_user_name(head, u1, "user b");
//     log_all_user(head, 1);

//     delete_user(u0);
//     log_all_user(head, 1);

//     User *u2 = user_init();
//     u2->data->id = get_min_id(head);
//     add_user(head, u2);
//     log_all_user(head, 0);
//     return 0;
// }