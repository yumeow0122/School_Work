#ifndef DB_API_H_INCLUDED
#define DB_API_H_INCLUDED

/**
 * @brief set user password to redis server
 * @param redis redis context
 * @param username username
 * @param password password
 * @return 1 if success, -1 if failed
 **/
int set_user_password(redisContext *redis, const char *username, const char *password);

/**
 * @brief get user password from redis server
 * @param redis redis context
 * @param username username
 * @param password password
 * @return 1 if success, -1 if failed
*/
int get_user_password(redisContext *redis, const char *username, char *password);
#endif // DB_API_H_INCLUDED
