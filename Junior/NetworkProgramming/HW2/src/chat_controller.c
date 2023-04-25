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

#include "util.h"
#include "shell_service.h"
#include "command_controller.h"
#include "chat_controller.h"

void *chat_client(void *args)
{
    ChatArgs *cargs = (ChatArgs *)args;
    int socketFD = cargs->socketFD;
    User *user = cargs->user;
    User *uhead = cargs->uhead;

    start_shell(socketFD);
    // Handle client connection here
    while (1)
    {
        char *sign = "%";
        char *input = malloc(MAX_INPUT_SIZE * sizeof(char *));
        char *output = malloc(MAX_OUTPUT_SIZE * sizeof(char *));

        log_all_user(uhead, user->data->id);
        send_msg(socketFD, sign);

        int num_bytes = recv(socketFD, input, MAX_INPUT_SIZE, 0);
        if (num_bytes == -1)
        {
            perror("recv");
            break;
        }
        else if (num_bytes == 0)
        {
            printf("Client disconnected\n");
            break;
        }

        crlf_to_lf(input);
        printf("Received message from client: %s\n", input);

        int state = shell(uhead, user, input, output, socketFD);
        strcat(output, "\n");
        if (state == -1)
            break;
        else if (state == 0)
            send_msg(socketFD, output);
    }
    end_shell(socketFD);
    delete_user(user);
    close(socketFD);
}

void send_msg(int fd, char *msg)
{
    if (send(fd, msg, strlen(msg), 0) == -1)
    {
        perror("send");
        close(fd);
        exit(1);
    }
}