#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include/IOController.c"
#include "../include/commandController.c"

#ifndef COMMAND_SIZE
#define COMMAND_SIZE 5000
#endif

char internal_command[100][100];
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
            strcpy(internal_command[internal_command_count], dir->d_name);
            internal_command_count++;
        }
    }
    closedir(d);
}

void initial()
{
    printf("Enter 'quit' to exit the shell.\n");
    printf("----- Shell Start. -----\n");

    set_internal_command();
}

void end()
{
    printf("----- Shell End. -----\n");
}

int main(int argc, char *argv[])
{
    char command[COMMAND_SIZE];

    initial();

    while ((1))
    {
        // get input command and status code of command
        int status = input(command);
        if (status == -1)
            break;

        // parse input command
        char **args = malloc(COMMAND_SIZE * sizeof(char *));
        int argc = command_parse(command, args);

        int mode = get_mode(argc, args);
        if (mode == 0)
        {
            printf("pipe mode.\n");
        }
        else
        {
            printf("non-pipe mode.\n");
            run_external(argc, args);
        }
    }

    end();

    return EXIT_SUCCESS;
}