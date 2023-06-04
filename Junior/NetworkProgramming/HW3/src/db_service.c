#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h>
#include <hiredis/hiredis.h>
#include <time.h>
#include <pthread.h>

#include "shell_service.h"
#include "chat_service.h"
#include "chat_utils.h"
#include "db_api.h"
#include "db_service.h"
#include "user_controller.h"

void *db_client(void *args)
{
    DbArgs *dbArgs = (DbArgs *)args;
    redisContext *redis = connect_redis();

    // login/register state
    char *username = malloc(sizeof(char) * MAX_USERNAME_LEN);
    username = user_login(redis, dbArgs->socketFD);
    dbArgs->user->data->name = username;
    add_user(dbArgs->uhead, dbArgs->user);

    // start db kernal
    ChatArgs *cargs = (ChatArgs *)malloc(sizeof(ChatArgs));
    cargs->socketFD = dbArgs->socketFD;
    cargs->user = dbArgs->user;
    cargs->uhead = dbArgs->uhead;
    chat_client(cargs);
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

char *user_login(redisContext *redis, int socketFD)
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
            free(password);
            return username;
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

void mailto(int userfd, char *sender, Command *cmd)
{
    char *receiver = cmd->args[1];
    char *message = malloc(sizeof(char) * 1024);
    for (int idx = 2; idx < cmd->argc; idx++)
    {
        strcat(message, cmd->args[idx]);
        strcat(message, " ");
    }

    redisContext *redis = connect_redis();
    redisReply *reply = (redisReply *)redisCommand(redis, "GET %s", receiver);
    if (reply == NULL || reply->type != REDIS_REPLY_STRING)
    {
        char msg[] = "User not found\n";
        send_msg(userfd, msg);
        freeReplyObject(reply);
        return;
    }
    freeReplyObject(reply);

    // get current time
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char current_time[256] = {};
    strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", tm);

    // output redirect
    if (message[0] == '<')
    {
        printf("do it later QQ\n");
    }

    char *identifier = (char *)malloc(sizeof(char) * 256);
    sprintf(identifier, "%s=>MailBox", receiver);

    // get suitable id
    reply = redisCommand(redis, "LLEN %s", identifier);
    long long id = 1;
    if (reply && reply->type == REDIS_REPLY_INTEGER)
        id = reply->integer;
    freeReplyObject(reply);

    char *buffer = (char *)malloc(sizeof(char) * 1024);
    sprintf(buffer, " %lld\t%s   %s \t\t%s\n", id, current_time, sender, message);

    reply = redisCommand(redis, "LPUSH %s %s", identifier, buffer);
    if (reply && reply->type == REDIS_REPLY_INTEGER && reply->integer != 0)
    {

        sprintf(buffer, "Send accept!\n");
        send_msg(userfd, buffer);
    }
    else
    {
        sprintf(buffer, "Send failed!\n");
        send_msg(userfd, buffer);
    }
    free(buffer);
    free(identifier);
    freeReplyObject(reply);
}

void listMail(int userfd, char *username)
{
    redisContext *redis = connect_redis();

    redisReply *reply = redisCommand(redis, "LRANGE %s=>MailBox 0 -1", username);
    if (reply)
    {
        if (reply->type == REDIS_REPLY_ARRAY && reply->elements != 0)
        {
            char prompt[] = "<id>\t<date>\t\t\t<sender>\t<message>\n";
            send_msg(userfd, prompt);
            redisReply **r = reply->element;
            size_t len = reply->elements;
            for (int i = 0; i < len; i++)
            {
                send_msg(userfd, r[i]->str);
            }
        }
        else if (reply->type == REDIS_REPLY_ARRAY && reply->elements == 0)
        {
            send_msg(userfd, "empty !\n");
        }
    }
    freeReplyObject(reply);
}

void delMail(int userfd, char *username, Command *cmd)
{
    redisContext *redis = connect_redis();
    char *id = cmd->args[1];
    redisReply *reply = redisCommand(redis, "LRANGE %s=>MailBox 0 -1", username);
    if (reply && reply->type == REDIS_REPLY_ARRAY)
    {

        redisReply **r = reply->element;
        size_t len = reply->elements;
        int i = 0;
        char delMsg[4096];
        char check[4];
        for (int i = 0; i < len; i++)
        {
            sscanf(r[i]->str, " %s", check);
            if (strcmp(check, id) == 0)
            {
                strcpy(delMsg, r[i]->str);
                break;
            }
        }
        freeReplyObject(reply);
        reply = redisCommand(redis, "LREM %s=>MailBox 0 %s", username, delMsg);
        if (reply->integer == 1)
        {
            send_msg(userfd, "Delete success!\n");
        }
        else
        {
            send_msg(userfd, "Delete failed!\n");
        }
    }
    else
    {
        send_msg(userfd, "Delete failed!\n");
    }
    freeReplyObject(reply);
}

void createGroup(int userfd, char *username, char *groupName)
{
    redisContext *redis = connect_redis();
    redisReply *reply = redisCommand(redis, "ZCARD groupList");
    int gid = reply->integer;
    freeReplyObject(reply);

    reply = redisCommand(redis, "ZADD groupList %d %s", gid + 1, groupName);

    if (reply->integer)
    {
        redisReply *tr = redisCommand(redis, "ZADD %s 1 %s", groupName, username);
        freeReplyObject(tr);
        char msg[] = "Create success !\n";
        send_msg(userfd, msg);
    }
    else
    {
        char msg[] = "Group already exist !\n";
        send_msg(userfd, msg);
    }
    freeReplyObject(reply);
}

void delGroup(int userfd, char *username, char *groupName)
{
    redisContext *redis = connect_redis();
    redisReply *reply = redisCommand(redis, "ZRANGE %s 0 -1", groupName);

    if (reply && reply->type == REDIS_REPLY_ARRAY)
    {
        if (reply->elements == 0)
        {
            char msg[] = "Group not found !\n";
            send_msg(userfd, msg);
            // write(userfd, msg, strlen(msg));
            freeReplyObject(reply);
            return;
        }

        if (strcmp(reply->element[0]->str, username) == 0)
        {
            redisReply *tr = redisCommand(redis, "DEL %s", groupName);
            char msg[] = "Group delete success !\n";
            send_msg(userfd, msg);
            // write(userfd, msg, strlen(msg));
            freeReplyObject(tr);

            tr = redisCommand(redis, "ZREM groupList %s", groupName);
            freeReplyObject(tr);
        }
        else
        {
            char msg[] = "You don't have permissions !\n";
            send_msg(userfd, msg);
            // write(userfd, msg, strlen(msg));
        }
    }
    freeReplyObject(reply);
}

void listGroup(int sockfd, char *username)
{
    redisContext *redis = connect_redis();
    redisReply *reply = redisCommand(redis, "ZRANGE groupList 0 -1");

    char prompt[] = " <owner> \t <group>  \n";
    int flag = 0;

    if (reply && reply->type == REDIS_REPLY_ARRAY)
    {
        size_t len = reply->elements;
        redisReply **r = reply->element;
        redisReply *reply_nest;
        char owner[64] = {};
        char group[64] = {};
        int first = 1;
        for (int i = 0; i < len; i++)
        {
            reply_nest = redisCommand(redis, "ZRANGE %s 0 -1", r[i]->str);
            if (reply_nest && reply_nest->type == REDIS_REPLY_ARRAY)
            {
                size_t lrn = reply_nest->elements;
                if (lrn > 0)
                {
                    redisReply **rn = reply_nest->element;
                    memset(group, 0, sizeof(group));
                    memset(owner, 0, sizeof(owner));
                    strcpy(group, r[i]->str);
                    strcpy(owner, rn[0]->str);
                    char msg[256] = {};
                    sprintf(msg, " %s \t %s  \n", owner, group);
                    for (int j = 0; j < lrn; j++)
                    {
                        if (strcmp(rn[j]->str, username) == 0)
                        {
                            if (first)
                            {
                                send_msg(sockfd, prompt);
                                // write(sockfd, prompt, strlen(prompt));
                                first = 0;
                            }
                            flag = 1;
                            send_msg(sockfd, msg);
                            // write(sockfd, msg, strlen(msg));
                            break;
                        }
                    }
                }
            }
            freeReplyObject(reply_nest);
        }
    }

    if (flag == 0)
    {
        send_msg(sockfd, "Empty !\n");
        // write(sockfd, "Empty !\n", strlen("Empty !\n"));
    }

    freeReplyObject(reply);
}

void leaveGroup(int sockfd, char *username, char *groupName)
{
    redisContext *redis = connect_redis();
    redisReply *reply = redisCommand(redis, "ZSCAN groupList 0 match %s", groupName);

    if (!reply->element[1]->element)
    {
        char msg[] = "Group not found !\n";
        send_msg(sockfd, msg);
        // write(sockfd, msg, strlen(msg));
        freeReplyObject(reply);
        return;
    }

    freeReplyObject(reply);
    redisReply *tr = redisCommand(redis, "ZRANGE %s 0 -1", groupName);
    size_t numberOfpeople = tr->elements;
    freeReplyObject(tr);
    reply = redisCommand(redis, "ZREM %s %s", groupName, username);

    if (reply && reply->integer == 0)
    {
        char msg[] = "Leave fault !\n";
        send_msg(sockfd, msg);
        // write(sockfd, msg, strlen(msg));
        freeReplyObject(reply);
        return;
    }

    if (numberOfpeople == 1)
    {
        tr = redisCommand(redis, "ZREM groupList %s", groupName);
        freeReplyObject(tr);
    }

    char msg[] = "Leave accept !\n";
    send_msg(sockfd, msg);
    // write(sockfd, msg, strlen(msg));
    freeReplyObject(reply);
}

void addTo(int sockfd, char *username, Command *cmd)
{
    char *groupName = cmd->args[1];
    char success[100][64] = {};
    char notFound[100][64] = {};
    char inGroup[100][64] = {};
    int pos = 0, count = 0, nextPos = 0;
    int sc = 0, nf = 0, ig = 0;
    redisReply *tr;

    redisContext *redis = connect_redis();
    redisReply *reply = redisCommand(redis, "ZRANGE %s 0 -1", groupName);
    if (reply->type == REDIS_REPLY_ARRAY)
    {
        if (reply->elements == 0)
        {
            char msg[] = "Group Not Found !\n";
            send_msg(sockfd, msg);
            // write(sockfd, msg, strlen(msg));
            freeReplyObject(reply);
            return;
        }

        if (strcmp(reply->element[0]->str, username) != 0)
        {
            char msg[] = "You don't have permissions !\n";
            send_msg(sockfd, msg);
            // write(sockfd, msg, strlen(msg));
            freeReplyObject(reply);
            return;
        }

        // user not found
        for (int i = 2; i < cmd->argc; i++)
        {
            tr = redisCommand(redis, "GET %s", cmd->args[i]);
            if (tr == NULL || tr->type != REDIS_REPLY_STRING)
            {
                strcpy(notFound[nf++], cmd->args[i]);
                strcpy(cmd->args[i], "");
            }
            freeReplyObject(tr);
        }

        // user already in group
        for (int i = 0; i < reply->elements; i++)
        {
            for (int j = 2; j < cmd->argc; j++)
            {
                if (strcmp(reply->element[i]->str, cmd->args[j]) == 0)
                {
                    strcpy(inGroup[ig++], cmd->args[j]);
                    strcpy(cmd->args[j], "");
                }
            }
        }

        // add user into group
        int gid = reply->elements + 1;
        for (int i = 2; i < cmd->argc; i++)
        {
            if (strcmp(cmd->args[i], "") != 0)
            {
                tr = redisCommand(redis, "ZADD %s %d %s", groupName, gid++, cmd->args[i]);
                strcpy(success[sc++], cmd->args[i]);
                freeReplyObject(tr);
            }
        }

        char prompt[1024] = {};

        // already in group msg
        for (int i = 0; i < ig; i++)
        {
            strcat(prompt, inGroup[i]);
            strcat(prompt, " ");
        }
        if (strcmp(prompt, "") != 0)
        {
            strcat(prompt, "already in group !\n");
            send_msg(sockfd, prompt);
            // write(sockfd, prompt, strlen(prompt));
        }

        // not found msg
        memset(prompt, 0, sizeof(prompt));
        for (int i = 0; i < nf; i++)
        {
            strcat(prompt, notFound[i]);
            strcat(prompt, " ");
        }
        if (strcmp(prompt, "") != 0)
        {
            strcat(prompt, "not found !\n");
            send_msg(sockfd, prompt);
            // write(sockfd, prompt, strlen(prompt));
        }

        // success msg
        memset(prompt, 0, sizeof(prompt));
        for (int i = 0; i < sc; i++)
        {
            strcat(prompt, success[i]);
            strcat(prompt, " ");
        }
        if (strcmp(prompt, "") != 0)
        {
            strcat(prompt, "add success !\n");
            send_msg(sockfd, prompt);
            // write(sockfd, prompt, strlen(prompt));
        }
    }
    freeReplyObject(reply);
}