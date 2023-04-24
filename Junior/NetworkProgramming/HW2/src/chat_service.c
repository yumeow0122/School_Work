#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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