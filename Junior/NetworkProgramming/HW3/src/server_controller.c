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

#include "server_service.h"
#include "server_controller.h"

void *server_client(void *arg)
{
    ServerArgs *sargs = (ServerArgs *)arg;
    int login_status = login(sargs->socketFD);
}