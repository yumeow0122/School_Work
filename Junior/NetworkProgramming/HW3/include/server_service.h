#ifndef SERVER_SERVICE_H_INCLUDED
#define SERVER_SERVICE_H_INCLUDED

int login(int fd);

int account_compare(char *username, char *password);
#endif // SERVER_SERVICE_H_INCLUDED
