#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>

#include "chat_service.h"
#include "db_api.h"
#include "db_service.h"
#include "user_controller.h"

void *db_client(void *args)
{
    DbArgs *dbArgs = (DbArgs *)args;
    redisContext *redis = connect_redis();

    // login/register state
    user_login(redis, dbArgs->socketFD);
    add_user(dbArgs->uhead, dbArgs->user);

    redisFree(redis);
}

redisContext *connect_redis()
{
    redisContext *redis = redisConnect("127.0.0.1", 6379);

    if (redis == NULL || redis->err)
    {
        if (redis)
        {
            printf("Error connecting to Redis: %s\n", redis->errstr);
            redisFree(redis);
        }
        else
        {
            printf("Failed to allocate Redis context\n");
        }
        exit(0);
        return NULL;
    }
    printf("Connected to Redis.\n");
    return redis;
}

void user_login(redisContext *redis, int socketFD)
{
    while (1)
    {
        char *username = malloc(sizeof(char) * MAX_USERNAME_LEN);
        char *password = malloc(sizeof(char) * MAX_PASSWORD_LEN);

        send_msg(socketFD, "username: ");
        recv_msg(socketFD, username);
        send_msg(socketFD, "password: ");
        recv_msg(socketFD, password);

        int result = login_account(redis, username, password);
        if (result == 1)
        {
            send_msg(socketFD, "Login success!\n");
            free(username);
            free(password);
            break;
        }
        else if (result == 0)
        {
            send_msg(socketFD, "Password incorrect, please try again.\n");
            continue;
        }

        char *choice = malloc(sizeof(char) * 2);

        send_msg(socketFD, "Account not found!\n");
        send_msg(socketFD, "Create account or login again ? <1/2> : ");
        recv_msg(socketFD, choice);
        if (strcmp(choice, "2") == 0)
        {
            continue;
        }

        send_msg(socketFD, "username: ");
        recv_msg(socketFD, username);
        send_msg(socketFD, "password: ");
        recv_msg(socketFD, password);

        result = register_account(redis, username, password);

        if (result == 1)
        {
            send_msg(socketFD, "Register success!\n");
            continue;
        }
        else if (result == 0)
        {
            send_msg(socketFD, "Account already exists, please try again.\n");
            continue;
        }
        else
        {
            send_msg(socketFD, "Failed to execute command, please try again.\n");
            continue;
        }
        free(username);
        free(password);
        free(choice);
    }
}

int login_account(redisContext *redis, const char *username, const char *password)
{
    char *pres = malloc(sizeof(char) * 100);
    int result = get_user_password(redis, username, pres);
    if (result == -1)
        return -1;
    if (strcmp(pres, password) == 0)
        return 1;
    else
        return 0;
}

int register_account(redisContext *redis, const char *username, const char *password)
{
    redisReply *reply = (redisReply *)redisCommand(redis, "EXISTS user:%s:password", username);
    if (reply == NULL)
    {
        printf("Failed to execute EXISTS command\n");
        return -1;
    }

    if (reply->type == REDIS_REPLY_ERROR)
    {
        printf("Error executing EXISTS command: %s\n", reply->str);
        freeReplyObject(reply);
        return -1;
    }

    if (reply->integer == 1)
    {
        freeReplyObject(reply);
        return 0;
    }

    freeReplyObject(reply);
    int result = set_user_password(redis, username, password);
    if (result == -1)
        return -1;
    return 1;
}