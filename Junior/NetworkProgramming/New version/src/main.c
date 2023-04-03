#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "run.h"
#include "utils.h"
#include "dll.h"

int main(int argc, char **argv, char **envp)
{
    start_shell();
    dllNode_t *head = DLL_init();
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

        dllNode_t *ouput = DLL_init();
        char *cmdOut = malloc(MAX_COMMAND_SIZE * sizeof(char));
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
                dllNode_t *newPipe = node_arg_init(cnt, cmdOut);
                DLL_add_tail((dllNode_t *)newPipe, head);
                break;
            }


            cmdOut = run_command(cmd, cmdOut);

            printf("%d: ", idx);
            print_command(cmd);    
            printf("res: %s\n", cmdOut);        
            // if (idx == cmdc - 1)
            // {
            //     printf("res: %s\n", cmdOut);
            // }
        }
    }
    end_shell();
    return 0;
}