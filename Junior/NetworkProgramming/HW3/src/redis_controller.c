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
            printf("connect redis fail: %s\n", redis->errstr);
            redisFree(redis);
            exit(1);
        }
        else
        {
            printf("cann't connect with redis\n");
            exit(1);
        }
    }
    printf("connect success.\n");
    return redis;
}