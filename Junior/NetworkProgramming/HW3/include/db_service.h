#ifndef DB_SERVICE_H_INCLUDED
#define DB_SERVICE_H_INCLUDED

#define MAX_USERNAME_LEN 32
#define MAX_PASSWORD_LEN 32

typedef struct DbArgs
{
    int socketFD;
    User *uhead;
    User *user;
} DbArgs;

/**
 * @brief db service entry point
 * @return void
 **/
void *db_client(void *args);

/**
 * @brief connect to redis server
 * @return redis context
 **/
redisContext *connect_redis();

/**
 * @brief user login function
 * @param redis redis context
 * @return void
 **/
void user_login(redisContext *redis, int socketFD);

/**
 * @brief user login function
 * @param redis redis context
 * @param username username
 * @param password password
 * @return 1 if login success, 0 if password incorrect, -1 if account not found
 **/
int login_account(redisContext *redis, const char *username, const char *password);

/**
 * @brief user register function
 * @param redis redis context
 * @param username username
 * @param password password
 * @return 1 if register success, 0 if account already exists, -1 if failed to execute command
 **/
int register_account(redisContext *redis, const char *username, const char *password);
#endif // DB_SERVICE_H_INCLUDED
