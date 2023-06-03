

#include "chat_service.h"

void send_msg(int fd, char *msg)
{
    if (send(fd, msg, strlen(msg), 0) == -1)
    {
        perror("send");
        close(fd);
        exit(1);
    }
}

void recv_msg(int fd, char *msg)
{
    int state = recv(fd, msg, MAX_INPUT_SIZE, 0);
    if (state == -1)
    {
        perror("recv");
        close(fd);
        exit(1);
    }
    crlf_to_lf(msg);
}