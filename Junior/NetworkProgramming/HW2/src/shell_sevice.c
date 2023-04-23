#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "shell_service.h"
#include "command_controller.h"
#include "user_controller.h"
#include "pipe_controller.h"

int shell(User *user, char *input, char *output)
{
    int isPipe = 0;
    UserData *udata = user->data;
    Command **commands = get_command_array();
    int cmdc = command_parse(input, commands);

    if (cmdc == 0)
        return 0;
    else if (strcmp(commands[0]->args[0], "exit") == 0)
        return -1;

    // char *cmdOut = malloc(MAX_OUTPUT_SIZE * sizeof(char));
    have_pipe_cmd(udata->pipeHead, output);
    for (int idx = 0; idx < cmdc; idx++)
    {
        Command *cmd = commands[idx];

        if (strcmp(cmd->args[0], "setenv") == 0)
        {
            run_setenv(cmd);
            break;
        }
        else if (cmd->args[0][0] == '|')
        {
            int cnt = atoi(cmd->args[0] + 1);
            Pipe *npipe = pipe_init();
            npipe->data->pipeCnt = cnt;
            npipe->data->prevOutput = output;
            add_pipe(udata->pipeHead, npipe);
            isPipe = 1;
            //strcpy(output, "");
            break;
        }

        output = run_command(cmd, output);

        // if (idx == cmdc - 1)
        // {
        //     printf("%s\n", output);
        // }
    }
    if (strlen(output) > 0)
    {
        decrement_all_pipe(udata->pipeHead);
    }
    return isPipe;
}

void run_setenv(Command *command)
{
    if (command->argc != 3)
    {
        printf("setenv error: invalid argument.\n");
        return;
    }
    setenv(command->args[1], command->args[2], 1);
}

void start_shell()
{
    printf("----- Shell start, enter \"exit\" to leave -----\n");
    setenv("PATH", "/bin:./bin", 1);
}

void end_shell()
{
    printf("----- Shell end. -----\n");
}