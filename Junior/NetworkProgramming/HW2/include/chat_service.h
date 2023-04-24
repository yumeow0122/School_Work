#ifndef CHAT_H_INCLUDED
#define CHAT_H_INCLUDED

void send_msg(int fd, char *msg);
int recv_msg(char *msg);
#endif // CHAT_H_INCLUDED
