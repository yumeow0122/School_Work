#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <hiredis/hiredis.h>

#include "util.h"
#include "command_controller.h"
#include "chat_controller.h"
#include "server_service.h"
#include "user_controller.h"

int user_connect(int fd, redisContext *redis)
{
    while (1)
    {
        send_msg(fd, "Welcome to the server, please login.\n");
        int login_state = user_login(fd, redis);

        switch (login_state)
        {
        case 1:
            send_msg(fd, "Login success.\n");
            return 1;

        case 0:
            send_msg(fd, "Password incorrect, please try again.\n");
            continue;

        case -1:
            send_msg(fd, "Account not found.\n");
            send_msg(fd, "Create account or login again ? <1/2> :");
            char *input = malloc(sizeof(char) * MAX_INPUT_SIZE);
            recv_msg(fd, input);

            switch (input[0])
            {
            case '1':
                user_register(fd, redis);
                break;

            case '2':
                continue;

            default:
                send_msg(fd, "Error, please try again.\n");
                exit(1);
            }
            free(input);
            break;

        default:
            send_msg(fd, "Error, please try again.\n");
            exit(1);
        }
    }
}

/*
 *   return 1 if login success
 *   return 0 if password incorrect
 *   return -1 if account not found
 *   return -2 if error
 */
int user_login(int fd, redisContext *redis)
{

    char *username = malloc(sizeof(char) * MAX_INPUT_SIZE);
    char *password = malloc(sizeof(char) * MAX_INPUT_SIZE);
    send_msg(fd, "Username: ");
    recv_msg(fd, username);
    send_msg(fd, "Password: ");
    recv_msg(fd, password);

    int login_state = account_compare(redis, fd, username, password);
    free(username);
    free(password);
    return login_state;
}

/**
 *   return 1 if register success
 *   return 0 if username already exist
 *   return -1 if error
 **/
int user_register(int fd, redisContext *redis)
{
    char *username = malloc(sizeof(char) * MAX_INPUT_SIZE);
    char *password = malloc(sizeof(char) * MAX_INPUT_SIZE);

    send_msg(fd, "Please enter a username: ");
    recv_msg(fd, username);

    // 檢查使用者名稱是否已存在
    redisReply *reply = (redisReply *)redisCommand(redis, "GET %s", username);
    if (reply != NULL && reply->type == REDIS_REPLY_STRING)
    {
        freeReplyObject(reply);
        send_msg(fd, "Username already exists. Please try again.\n");
        return 0;
    }
    freeReplyObject(reply);

    send_msg(fd, "Please enter a password: ");
    recv_msg(fd, password);

    // 儲存帳戶資料
    reply = (redisReply *)redisCommand(redis, "SET %s %s", username, password);
    if (reply == NULL || reply->type == REDIS_REPLY_ERROR)
    {
        freeReplyObject(reply);
        send_msg(fd, "Error in creating account. Please try again.\n");
        return -1;
    }
    freeReplyObject(reply);

    send_msg(fd, "Account created successfully.\n");
    return 1;
}

/*
 *   return 1: login success
 *   return 0: password incorrect
 *   return -1: account not found
 */
int account_compare(redisContext *redis, int fd, char *username, char *password)
{
    redisReply *reply = redisCommand(redis, "GET popo");
    printf("reply->str: %ld\n", reply->len);
    return 0;

    // redisReply *reply = redisCommand(redis, "GET %s", username);
    // if (reply == NULL)
    // {
    //     char *account_not_found = "Account not found, please try again.";
    //     send_msg(fd, account_not_found);
    //     return -1;
    // }
    // else if (reply->type == REDIS_REPLY_NIL)
    // {
    //     freeReplyObject(reply);
    //     char *account_not_found = "Account not found, please try again.";
    //     send_msg(fd, account_not_found);
    //     return -1;
    // }
    // // else if (strcmp(reply->str, password) == 0)
    // // {
    // //     char *login_success = "Login success.";
    // //     send_msg(fd, login_success);
    // //     return 1;
    // // }
    // else
    // {
    //     printf("password: %s\n", reply->str);
    //     char *password_incorrect = "Password incorrect, please try again.";
    //     send_msg(fd, password_incorrect);
    //     return 0;
    // }
}