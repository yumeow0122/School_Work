#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>

#include "db_api.h"

int set_user_password(redisContext *redis, const char *username, const char *password)
{
    // 儲存帳戶資料
    redisReply *reply = (redisReply *)redisCommand(redis, "SET %s %s", username, password);
    if (reply == NULL || reply->type == REDIS_REPLY_ERROR)
    {
        freeReplyObject(reply);
        return -1;
    }
    freeReplyObject(reply);
    return 1;
}

int get_user_password(redisContext *redis, const char *username, char *password)
{
    redisReply *reply = redisCommand(redis, "GET %s", username);
    if (reply == NULL || reply->type == REDIS_REPLY_NIL)
    {
        return -1;
    }

    strcpy(password, reply->str);
    freeReplyObject(reply);
    return 1;
}