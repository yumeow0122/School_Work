#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include "command.h"

#define READ_END 0
#define WRITE_END 1

#define die(e)                      \
    do                              \
    {                               \
        fprintf(stderr, "%s\n", e); \
        exit(EXIT_FAILURE);         \
    } while (0);

void sigpipe_handler(int signum)
{
    /**
     * When commaand not found or some error happen,
     * second child try to write data to parent but it already close.
     *
     * OS will genearate "SIGPIPE" and output something by default,
     * use "signal(SIGPIPE, sigpipe_handler);"
     * to make operation be "sigpipe_handler".
     */
}

int command_parse(char *command, Command **commands)
{
    int cmdc = 0;
    char *delim = " \n";
    char *arg = strtok(command, delim);
    if (arg == NULL)
        return 0;

    while (arg != NULL)
    {
        if (arg[0] == '|')
        {
            cmdc++;
            if (strlen(arg) > 1)
            {
                int argc = commands[cmdc]->argc;
                commands[cmdc]->args[argc] = arg;
                commands[cmdc]->argc++;
            }
        }
        else
        {
            int argc = commands[cmdc]->argc;
            commands[cmdc]->args[argc] = arg;
            commands[cmdc]->argc++;
        }
        arg = strtok(NULL, delim);
    }
    return cmdc + 1;
}

Command **build_command_array()
{
    Command **commands = malloc(MAX_COMMAND_SIZE * sizeof(Command *));
    for (int i = 0; i < MAX_COMMAND_SIZE; i++)
    {
        commands[i] = malloc(sizeof(Command));
        commands[i]->argc = 0;
        commands[i]->args = malloc(MAX_COMMAND_SIZE * sizeof(char *));
        for (int j = 0; j < MAX_COMMAND_SIZE; j++)
        {
            commands[i]->args[j] = NULL;
        }
    }
    commands[MAX_COMMAND_SIZE - 1] = NULL;

    return commands;
}

char *run_command(Command *command, char *prevOut) {
    signal(SIGPIPE, sigpipe_handler);
    int save_in;
    save_in = dup(STDIN_FILENO);

    int link[2];
    pid_t pid;
    char *res = malloc(MAX_COMMAND_SIZE * sizeof(char));

    if (pipe(link) == -1)
        die("pipe1");

    if ((pid = fork()) == -1)
        die("fork");

    if (pid == 0) {
        // child
        if (dup2(link[1], STDOUT_FILENO) == -1)
            die("dup2");

        if (prevOut != NULL) {
            if (dup2(STDOUT_FILENO, STDIN_FILENO) == -1)
                die("dup2");
            write(STDIN_FILENO, prevOut, strlen(prevOut));
        }

        close(link[0]);
        close(link[1]);

        // Set SIGPIPE handler
        signal(SIGPIPE, sigpipe_handler);

        execvp(command->args[0], command->args);
        die("execvp");
    } else {
        // parent
        close(link[1]);
        waitpid(pid, NULL, 0);
        int len = read(link[0], res, MAX_COMMAND_SIZE);
        if (len == -1)
            die("read");
        res[len] = '\0';
    }
    dup2(save_in, STDIN_FILENO);
    return res;
}


void print_command(Command *command)
{
    for (int c = 0; c < command->argc; c++)
        printf("%s ", command->args[c]);
    printf("\n");
}