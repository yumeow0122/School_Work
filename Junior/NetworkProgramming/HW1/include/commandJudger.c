#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "./util.c"


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

int number_pipe(int argc, char **args)
{
    int numberPipe = 0;
    int len = strlen(args[argc-1]);
    if(args[argc-1][0] == '|' && len > 1){
        char *num = malloc(COMMAND_SIZE * sizeof(char));
        substr(num, args[argc-1], 1, len-1);
        return atoi(num);
    }
    return -1;
}