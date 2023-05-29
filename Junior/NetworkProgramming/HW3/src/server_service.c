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
#include "command_controller.h"
#include "chat_controller.h"
#include "server_service.h"
#include "user_controller.h"

int login(int fd)
{
    while (1)
    {
        char *login_msg1 = "user name:";
        char *login_msg2 = "password:";

        char *input = malloc(MAX_INPUT_SIZE * sizeof(char *));
        char *output = malloc(MAX_OUTPUT_SIZE * sizeof(char *));

        send_msg(fd, login_msg1);
        recv_msg(fd, input);
        printf("Received account from client: %s\n", input);

        send_msg(fd, login_msg2);
        recv_msg(fd, input);
        printf("Received password from client: %s\n", input);
    }
}

int account_compare(char *username, char *password)
{
    
}