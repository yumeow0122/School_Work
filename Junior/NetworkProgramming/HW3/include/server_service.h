#ifndef SERVER_SERVICE_H_INCLUDED
#define SERVER_SERVICE_H_INCLUDED

int user_connect(int fd, redisContext *redis);

int user_login(int fd, redisContext *redis);

int user_register(int fd, redisContext *redis);

int account_compare(redisContext *redis, int fd, char *username, char *password);
#endif // SERVER_SERVICE_H_INCLUDED
