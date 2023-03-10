#include <stdio.h>
#include <stdlib.h>

#ifndef COMMAND_SIZE
#define COMMAND_SIZE 5000
#endif

// get user command and return the status code of command
int input(char *command)
{
    printf("%% ");
    fgets(command, 5000, stdin);
    if (strcmp(command, "exit\n") == 0 || strcmp(command, "quit\n") == 0)
    {
        return -1; // exit code
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
        args[argc++] = arg;
        arg = strtok(NULL, delim);
    }
    args[argc] = NULL;
    return argc;
}

int get_mode(int argc, char **args)
{
    if (argc > 1)
    {
        for (int i = 0; i < argc; i++)
            if (strcmp(args[i], "|") == 0)
                return 0;
    }
    return 1;
}
