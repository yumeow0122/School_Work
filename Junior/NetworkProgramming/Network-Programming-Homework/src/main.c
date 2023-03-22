#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <ctype.h>

#define PREVIOUS_COMMAND_OUTPUT_SIZE 32768
#define die(e)                      \
    do                              \
    {                               \
        fprintf(stderr, "%s\n", e); \
        exit(EXIT_FAILURE);         \
    } while (0);

char previousCommandOutput[PREVIOUS_COMMAND_OUTPUT_SIZE];

// use for number pipe
char savedCommandOutput[PREVIOUS_COMMAND_OUTPUT_SIZE];

#include "../include/commandParse.c"
#include "../include/runCommand.c"

void init()
{
    printf("Welcome to the shell!\n");
    printf("Type 'quit' or 'exit' to exit the shell.\n");

    memset(previousCommandOutput, 0, PREVIOUS_COMMAND_OUTPUT_SIZE);

    // mark this line if you want to use the system executables.
    setenv("PATH", "/bin:./bin", 1);
}

int main(int argc, char **argv, char **envp)
{
    char command[MAX_COMMANDS_SIZE];
    char quit[] = "quit";
    char _exit[] = "exit";
    char _setenv[] = "setenv";
    char printenv[] = "printenv";

    int pipeCounter = 0;

    init();
    while (1)
    {
        char **splitedCommand = malloc(MAX_COMMANDS_SIZE * sizeof(char *));
        int piped = 0;
        printf("%% ");

        if (fgets(command, MAX_COMMANDS_SIZE, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        int splitedCommandCount = command_parse(command, splitedCommand);
        if (splitedCommandCount == 0)
            continue;

        int currentCommandIndex = 0;
        if (strcmp(splitedCommand[0], quit) == 0 || strcmp(splitedCommand[0], _exit) == 0)
            break;

        if (strcmp(splitedCommand[0], _setenv) == 0)
        {
            run_setenv(splitedCommandCount, splitedCommand);
            continue;
        }

        for (int i = 0; i < splitedCommandCount; i++)
        {
            /**
             * Build the command
             *
             * Pipe all previous command output to the current command,
             * it doesn't take effect if there is no previous command output or the current command is not accept input.
             *
             * Pipe counter:
             * 0: no pipe
             * 1: current command is piped
             * >1: command output is queued to be piped
             */
            pipeCounter = (pipeCounter < 1) ? 0 : pipeCounter - 1;

            int cnt = 0;
            char **curCommand = malloc(MAX_COMMANDS_SIZE * sizeof(char *));
            while (i < splitedCommandCount && strstr(splitedCommand[i], "|") == NULL)
            {
                curCommand[cnt++] = splitedCommand[i++];
            }

            run_command(curCommand);

            /**
             * Check if the command contains pipe
             * If present, save the output of the command to savedCommandOutput.
             */
            if (splitedCommand[i] != NULL)
            {
                char *pipCount = strchr(splitedCommand[i], '|');
                if (pipCount != NULL)
                {
                    pipeCounter = (strlen(pipCount) > 1) ? atoi(&pipCount[1]) : 1;
                    strcpy(savedCommandOutput, previousCommandOutput);
                    piped = 1;
                }
            }
        }

        /**
         * If command not found, length of previousCommandOutput will be 0.
         * If the command is piped, not print to the console
         * If the next command is the piped command, override the previousCommandOutput with savedCommandOutput
         *
         * Pipcounter:
         * 0: no pipe
         * 1: next command is piped
         * >1: ignore
         */
        // printf("previousCommandOutput: %d\n", strlen(previousCommandOutput));
        if (strlen(previousCommandOutput) > 0)
        {
            if (pipeCounter == 0)
            {
                printf("%s\n", previousCommandOutput);
                memset(previousCommandOutput, 0, PREVIOUS_COMMAND_OUTPUT_SIZE);
            }
            if (pipeCounter != 0 && !piped)
                printf("%s\n", previousCommandOutput);
            if (pipeCounter == 1)
            {
                memset(previousCommandOutput, 0, PREVIOUS_COMMAND_OUTPUT_SIZE);
                strcat(previousCommandOutput, savedCommandOutput);
                memset(savedCommandOutput, 0, PREVIOUS_COMMAND_OUTPUT_SIZE);
            }
        }
        else
        {
            // command not found will not consume pipeCounter
            if (pipeCounter != 0)
                pipeCounter++;
        }
    }
}