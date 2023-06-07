#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server.h"
#include "message.h"

#define STUDENT_ID "410921237"

int main()
{
    printf("===== server connected =====\n");
    int socketFd = server_connect();

    Msgbuf msg;
    msg.mtype = 101;
    strcpy(msg.context, STUDENT_ID);

    printf("send connect message to server.\n\n");
    if (write(socketFd, &msg, sizeof(struct Msgbuf)) < 0)
    {
        perror("write");
        close(socketFd);
        exit(EXIT_FAILURE);
    }

    printf("receive message from server:\n");
    if (read(socketFd, &msg, sizeof(struct Msgbuf)) < 0)
    {
        perror("read");
        close(socketFd);
        exit(EXIT_FAILURE);
    }
    printf("type: %d\n", msg.mtype);
    printf("context: %s\n\n", msg.context);
    if (msg.mtype == 402)
    {
        printf("student ID is not inside data base.\n");
        exit(EXIT_FAILURE);
    }

    printf("send xor message to server.\n\n");
    msg.mtype = 300;
    xor(&msg, &STUDENT_ID[1]);
    if (write(socketFd, &msg, sizeof(struct Msgbuf)) < 0)
    {
        perror("write");
        close(socketFd);
        exit(EXIT_FAILURE);
    }

    printf("receive test result from server:\n");
    if (read(socketFd, &msg, sizeof(struct Msgbuf)) < 0)
    {
        perror("read");
        close(socketFd);
        exit(EXIT_FAILURE);
    }
    if (msg.mtype == 203)
    {
        printf("test pass.\n");
    }
    else
    {
        printf("test fail.\n");
    }
    
    close(socketFd);
    return 0;
}