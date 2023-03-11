#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

char internal_commands[100][100];
int internal_command_count = 0;

void set_internal_command()
{
    // scan the bin directory for executables, use it if exist, or else use the system executables.
    DIR *d;
    struct dirent *dir;
    d = opendir("./bin");
    if (d == NULL)
        perror("Open Directory ERROR:");

    while ((dir = readdir(d)) != NULL)
    {
        // type of file is regular
        if (dir->d_type == DT_REG)
        {
            strcpy(internal_commands[internal_command_count], dir->d_name);
            internal_command_count++;
        }
    }
    closedir(d);
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

int pipe_count(int argc, char **args)
{
    int pipeCount = 0;
    for (int i = 0; i < argc; i++)
        if (strcmp(args[i], "|") == 0)
            pipeCount++;
    return pipeCount;
}

int is_internal_commnad(char *arg)
{
    for (int i = 0; i < internal_command_count; i++)
        if (strcmp(arg, internal_commands[i]) == 0)
            return 1;
    return 0;
}

int get_printenv_mode(int argc, char **args)
{
    // too many argument for printenv
    if(argc > 2 && strcmp(args[2], "|") != 0)
        return -1;

    // normal format or with pipe
    else if(argc < 2 || strcmp(args[1], "|") == 0)
        return 0;

    // to get specific value in env
    else
        return 1;
}