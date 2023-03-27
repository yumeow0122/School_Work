#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "run.h"
#include "utils.h"

int main(int argc, char **argv, char **envp)
{
    start_shell();
    while (1)
    {
        printf("%% ");
        char *input = malloc(MAX_COMMAND_SIZE * sizeof(char));
        if (fgets(input, MAX_COMMAND_SIZE, stdin) == NULL)
        {
            printf("\n");
            return 0;
        }

        Command **commands = build_command_array();
        int cmdc = command_parse(input, commands);

        if (cmdc == 0)
            continue;
        else if (strcmp(commands[0]->args[0], "exit") == 0)
            break;

        for (int idx = 0; idx < cmdc; idx++)
        {
            Command *cmd = commands[idx];
            if (strcmp(cmd->args[0], "setenv") == 0)
            {
                run_setenv(cmd);
                break;
            }
        }
    }
    end_shell();
    return 0;
}