#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifndef COMMAND_SIZE
#define COMMAND_SIZE 5000
#endif

char existBin[100][100];
char existBinCount = 0;

void initial()
{
    printf("Enter 'quit' to exit the shell.\n");
    printf("----- Shell Start. -----\n");

    // scan the bin directory for executables, use it if exist, or else use the system executables.
    DIR *d;
    struct dirent *dir;
    d = opendir("./bin");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {
                strcpy(existBin[existBinCount], dir->d_name);
                existBinCount++;
                printf("%s\n", dir->d_name);
            }
        }
        closedir(d);
    }
}

void end()
{
    printf("----- Shell End. -----\n");
}

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
}

void run(int argc, char **args)
{
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
        run(argc, args);
    }

    end();

    return EXIT_SUCCESS;
}