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
    char **preArgs = malloc(COMMAND_SIZE * COMMAND_SIZE * sizeof(char));
    initial();

    while (input(command))
    {
        // get input command and status code of command
        // parse input command
        char **args = malloc(COMMAND_SIZE * COMMAND_SIZE * sizeof(char));
        int argc = command_parse(command, args);
        if (argc == 0)
            continue;

        if (numberPipe == 0)
        {
            int preArgc;
            for (preArgc = 0; preArgs[preArgc] != NULL; preArgc++);

            free(preArgs[preArgc--]);
            strcpy(preArgs[preArgc++], "|");
            for (int idx = 0; idx < argc; idx++)
            {
                preArgs[preArgc++] = strdup(args[idx]);
            }

            run_command(preArgc, preArgs, envp, NULL);

            numberPipe = -1;
            continue;
        }
        run_command(argc, args, envp, preArgs);
        numberPipe--;
    }

    end();

    return EXIT_SUCCESS;
}