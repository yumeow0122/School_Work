#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include/IOController.c"

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

// // get user command and return the status code of command
// int input(char *command)
// {
//     printf("%% ");
//     fgets(command, 5000, stdin);
//     if (strcmp(command, "exit\n") == 0 || strcmp(command, "quit\n") == 0)
//     {
//         return -1; // exit code
//     }
//     return 1;
// }

// int command_parse(char *command, char **args)
// {
//     int argc = 0;
//     char *arg = malloc(COMMAND_SIZE * sizeof(char));
//     char *delim = " \n";

//     arg = strtok(command, delim);
//     while (arg != NULL)
//     {
//         args[argc++] = arg;
//         arg = strtok(NULL, delim);
//     }
//     args[argc] = NULL;
//     return argc;
// }

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

void run_external(int argc, char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execvp(args[0], args) == -1)
        {
            perror("external command eroor:");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Error forking
        perror("external command eroor:");
    }
    else
    {
        // Parent process
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
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
            printf("internal\n");
        }
        else
        {
            printf("external\n");
            run_external(argc, args);
        }
    }

    end();

    return EXIT_SUCCESS;
}