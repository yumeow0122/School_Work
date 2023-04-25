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
#include <pthread.h>

#include "util.h"
#include "user_controller.h"
#include "command_controller.h"
#include "shell_service.h"
#include "chat_controller.h"

#define PORT 8888

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
    User *uhead = user_init();
    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1)
        {
            perror("accept");
            continue;
        }

        char *_userIP = inet_ntoa(their_addr.sin_addr);
        char *_userPort = int_to_str(ntohs(their_addr.sin_port));
        char *userIP = get_ip_port(_userIP, _userPort);

        printf("New client connected from %s\n", userIP);

        User *user = user_init();
        user->data->id = get_min_id(uhead);
        user->data->ip = userIP;
        add_user(uhead, user);


        ChatArgs *cargs = (ChatArgs *)malloc(sizeof(ChatArgs));
        cargs->socketFD = new_fd;
        cargs->user = user;
        cargs->uhead = uhead;
        
        pthread_t thread;
        if (pthread_create(&thread, NULL, chat_client, (void *)cargs) != 0)
        {
            perror("pthread_create");
            // close(new_fd);
            free(cargs);
            continue;
        }
        pthread_detach(thread);
        // close(new_fd);
    }

    return 0;
}
