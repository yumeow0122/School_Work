#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "message_controller.h"

void xor(Msgbuf *msg, char *id)
{
    int i;
    for (i = 0; i < 8; i++) {
        msg->context[i] = (msg->context[i] ^ id[i]);
        printf("%d", msg->context[i]);
    }
    printf("\n");
}