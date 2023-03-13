#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include "../include/IOController.c"
#include "../include/commandController.c"

void initial()
{
    set_internal_command();

    // set execute path to './bin'
    setenv("PATH", "/bin:./bin", 1);

    printf("Enter 'quit' to exit the shell.\n");
    printf("----- Shell Start. -----\n");
}

void end()
{
    printf("----- Shell End. -----\n");
}

int main(int argc, char *argv[], char **envp)
{
    char *command = malloc(COMMAND_SIZE * sizeof(char *));

    initial();

    while (input(command))
    {
        // get input command and status code of command
        // parse input command
        char **args = malloc(COMMAND_SIZE * sizeof(char *));
        int argc = command_parse(command, args);

        run_command(argc, args, envp);
    }

    end();

    return EXIT_SUCCESS;
}