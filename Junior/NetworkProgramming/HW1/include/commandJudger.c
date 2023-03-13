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