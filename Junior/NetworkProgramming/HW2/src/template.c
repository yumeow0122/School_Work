#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "user_controller.h"
#include "command_controller.h"
#include "pipe_controller.h"
#include "shell_service.h"

int test()
{
    User *user = user_init();
    start_shell();
    while (1)
    {
        char *input = malloc(MAX_INPUT_SIZE * sizeof(char));
        char *output = malloc(MAX_OUTPUT_SIZE * sizeof(char));

        printf("%% ");
        fgets(input, MAX_COMMAND_SIZE, stdin);

        int state = shell(user, input, output);
        if(state == -1)
            break;
        else if(state == 0)
            printf("%s\n", output);

        //log_all_pipe(user->data->pipeHead);
    }
    end_shell();
    return 0;
}
