#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>

#include "redis_controller.h"

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
        return NULL;
    }
    return redis;
}