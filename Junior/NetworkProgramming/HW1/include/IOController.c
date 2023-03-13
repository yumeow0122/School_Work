#include <stdio.h>
#include <stdlib.h>

#ifndef COMMAND_SIZE
#define COMMAND_SIZE 5000
#endif

// get user command and return the status code of command
int input(char *command)
{
    printf("%% ");
    if (fgets(command, 5000, stdin) == NULL)
    {
        printf("\n");
        return 0;
    }
    if (strcmp(command, "exit\n") == 0 || strcmp(command, "quit\n") == 0)
    {
        return 0; // exit code
    }
    return 1;
}

int command_parse(char *command, char **args)
{
    int argc = 0;
    char *arg = malloc(COMMAND_SIZE * sizeof(char));
    char *delim = " \n";

    arg = strtok(command, delim);
    while (arg != NULL)
    {
        arg += '\0';
        args[argc++] = arg;
        arg = strtok(NULL, delim);
    }
    args[argc] = NULL;
    return argc;
}