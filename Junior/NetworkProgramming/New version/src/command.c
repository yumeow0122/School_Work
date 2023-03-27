#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "command.h"

int command_parse(char *command, Command **commands)
{
    int cmdc = 0;
    char *delim = " \n";
    char *arg = strtok(command, delim);
    if (arg == NULL)
        return 0;

    while (arg != NULL)
    {
        if (arg[0] == '|')
        {
            cmdc++;
            if (strlen(arg) > 1)
            {
                int argc = commands[cmdc]->argc;
                commands[cmdc]->args[argc] = arg;
                commands[cmdc]->argc++;
            }
        }
        else
        {
            int argc = commands[cmdc]->argc;
            commands[cmdc]->args[argc] = arg;
            commands[cmdc]->argc++;
        }
        arg = strtok(NULL, delim);
    }
    return cmdc + 1;
}

Command **build_command_array()
{
    Command **commands = malloc(MAX_COMMAND_SIZE * sizeof(Command *));
    for (int i = 0; i < MAX_COMMAND_SIZE; i++)
    {
        commands[i] = malloc(sizeof(Command));
        commands[i]->argc = 0;
        commands[i]->args = malloc(MAX_COMMAND_SIZE * sizeof(char *));
        for (int j = 0; j < MAX_COMMAND_SIZE; j++)
        {
            commands[i]->args[j] = NULL;
        }
    }
    commands[MAX_COMMAND_SIZE - 1] = NULL;

    return commands;
}