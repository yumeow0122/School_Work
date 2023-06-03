#ifndef CHAT_SERVICE_H_INCLUDED
#define CHAT_SERVICE_H_INCLUDED

#define MAX_INPUT_SIZE 1024
/**
 * @brief: send message to user
 * @param fd: socket file descriptor
 * @param msg: message to send
 * @return: void
 **/
void send_msg(int fd, char *msg);

/**
 * @brief: receive message from user
 * @param fd: socket file descriptor
 * @param msg: message to receive
 * @return: void
 **/
void recv_msg(int fd, char *msg);

#endif // CHAT_SERVICE_H_INCLUDED