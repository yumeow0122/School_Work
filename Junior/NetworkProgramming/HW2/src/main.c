#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "shell.h"

#define PORT 5001

int main()
{
    int sockfd, new_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    socklen_t sin_size;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, 10) == -1)
    {
        perror("listen");
        exit(1);
    }

    printf("Server started and listening on port %d...\n", PORT);

    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
        {
            perror("accept");
            continue;
        }

        printf("New client connected from %s on port %d\n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port));

        if (fork() == 0)
        {
            close(sockfd);

            char *message = "hello\n";
            if (send(new_fd, message, strlen(message), 0) == -1)
            {
                perror("send");
                close(new_fd);
                exit(1);
            }

            char *inBuf = malloc(5000 * sizeof(char));
            // Handle client connection here
            while (1)
            {
                int num_bytes = recv(new_fd, inBuf, sizeof(inBuf) - 1, 0);
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

                inBuf[num_bytes] = '\0';

                char *outBuf = malloc(5000 * sizeof(char));
                int state = shell(inBuf, outBuf);
                printf("Received message from client: %s\n", inBuf);

                // Echo message back to client
                if (send(new_fd, outBuf, num_bytes, 0) == -1)
                {
                    perror("send");
                    break;
                }
            }

            close(new_fd);
            exit(0);
        }

        close(new_fd);
    }

    return 0;
}
