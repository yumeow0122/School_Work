#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server_connect.h"
#include "message_controller.h"

#define STUDENT_ID "410921237"

int main()
{
    int socketFd = server_connect();
    printf("server connected: %d\n", socketFd);

    Msgbuf msg;
    msg.mtype = 101;
    strcpy(msg.context, STUDENT_ID);

    if (write(socketFd, &msg, sizeof(struct Msgbuf)) < 0)
    {
        perror("write");
        close(socketFd);
        exit(EXIT_FAILURE);
    }
    printf("send connect massage to server.\n");

    if (read(socketFd, &msg, sizeof(struct Msgbuf)) < 0)
    {
        perror("read");
        close(socketFd);
        exit(EXIT_FAILURE);
    }
    printf("receive massage from server.\n");
    printf("msg: %d\n", msg.mtype);
    printf("msg: %s\n", msg.context);

    if (msg.mtype == 402)
    {
        printf("student ID is not inside data base.\n");
    }

    msg.mtype = 300;
    xor(&msg, "10921237");
    printf("send xor massage to server.\n");
    printf("msg: %d\n", msg.mtype);
    printf("msg: %s\n", msg.context);
    if (write(socketFd, &msg, sizeof(struct Msgbuf)) < 0)
    {
        perror("write");
        close(socketFd);
        exit(EXIT_FAILURE);
    }

    if (read(socketFd, &msg, sizeof(struct Msgbuf)) < 0)
    {
        perror("read");
        close(socketFd);
        exit(EXIT_FAILURE);
    }
    printf("receive massage from server.\n");
    printf("res: %d\n", msg.mtype);
    printf("res: %s\n", msg.context);

    close(socketFd);
    return 0;
}