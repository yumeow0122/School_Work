#include <ctype.h>
#include <signal.h>

#define READ_END 0
#define WRITE_END 1
int save_in;

/**
 * @brief run the command
 * There are three processes in this function which share 2 pipe.
 * The first will fork to a child process to run the command which read the stdin from second child and produce stdout to parent.
 * The second process is also the child process, that generate stdin for first child process.
 * The last process is the parent process, it will read the stdout from the first child process.
 *
 * link1: write echo output to command stdin  (child2 -> child1)
 * link2: read command output to parent stdin (child1 -> parent)
 *
 * @param command command to run
 */
int run_command(char **command)
{
    signal(SIGPIPE, SIG_IGN); // ignore SIGPIPE signal

    int save_in;
    save_in = dup(STDIN_FILENO);

    // copy previousCommandOutput to currentCommandOutput and clear previousCommandOutput
    char currentCommandOutput[PREVIOUS_COMMAND_OUTPUT_SIZE];
    memset(currentCommandOutput, 0, PREVIOUS_COMMAND_OUTPUT_SIZE);
    strcat(currentCommandOutput, previousCommandOutput);
    memset(previousCommandOutput, 0, PREVIOUS_COMMAND_OUTPUT_SIZE);

    int link1[2];
    int link2[2];
    pid_t pid1, pid2;
    char tempBuffer[4096 + 1];

    if (pipe(link1) == -1)
        die("pipe1");
    if (pipe(link2) == -1)
        die("pipe2");

    if ((pid1 = fork()) == -1)
        die("fork1");

    if (pid1 == 0)
    {
        // child 1

        // set I/O to link1 and link2
        dup2(link1[READ_END], STDIN_FILENO);
        dup2(link2[WRITE_END], STDOUT_FILENO);
        dup2(STDOUT_FILENO, STDERR_FILENO); // redirect standard error to standard output

        // close communcation between link1 and link2
        close(link1[WRITE_END]);
        close(link2[READ_END]);

        execvp(command[0], command);
        char *error = malloc(128 * sizeof(char));
        strcpy(error, command[0]);
        strcat(error, ": command not found");
        die(error);
    }
    else
    {
        if ((pid2 = fork()) == -1)
            die("fork2");
        if (pid2 == 0)
        {
            // child
            dup2(link1[WRITE_END], STDOUT_FILENO);

            close(link1[READ_END]);
            close(link2[READ_END]);
            close(link2[WRITE_END]);

            char *argv[] = {"./bin/echo", "-e", currentCommandOutput, NULL};
            execvp(argv[0], argv);
            die("echo: command not found");
        }
        else
        {
            // parent
            dup2(link2[READ_END], STDIN_FILENO);

            close(link1[READ_END]);
            close(link1[WRITE_END]);
            close(link2[WRITE_END]);

            int nbytes = 0;
            while (0 != (nbytes = read(link2[READ_END], tempBuffer, sizeof(tempBuffer))))
            {
                memset(previousCommandOutput, 0, PREVIOUS_COMMAND_OUTPUT_SIZE);
                strcat(previousCommandOutput, tempBuffer);
                memset(tempBuffer, 0, 4096);
            }
            // remove the last newline character if it exists
            size_t len = strlen(previousCommandOutput);
            if (len > 0 && previousCommandOutput[len - 1] == '\n')
                previousCommandOutput[len - 1] = '\0';

            // wait for child processes to finish
            wait(NULL);
        }
        // wait for child processes to finish
        wait(NULL);
    }
    // return to the original stdin for parent process
    dup2(save_in, STDIN_FILENO);
}

/**
 * @brief run the command "setenv"
 * "setenv" is not an original commands, so it needs to
 * handle with exception
 * @param command command to run
 */
void run_setenv(int splitedCommandCount, char **splitedCommand)
{
    if (splitedCommandCount < 3)
        printf("setenv: not enough arguments\n");
    else if (splitedCommandCount > 3)
        printf("setenv: too many arguments\n");
    else
        setenv(splitedCommand[1], splitedCommand[2], 1);
}