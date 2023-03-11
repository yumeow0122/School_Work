#include <sys/types.h>
#include <sys/wait.h>

#ifndef OUTPUT_SIZE
#define OUTPUT_SIZE 50000
#endif

char *build_printenv_output(int mode, int argc, char **args, char **envp)
{
    char *prevOutput = malloc(OUTPUT_SIZE * sizeof(char));
    memset(prevOutput, 0, OUTPUT_SIZE);
    if (mode == 0)
    {
        for (char **env = envp; *env != NULL; env++)
        {
            strcat(prevOutput, *env);
            strcat(prevOutput, "\n");
        }
        prevOutput[strlen(prevOutput) - 1] = '\0';
    }
    else if (mode == 1)
    {
        strcat(prevOutput, getenv(args[1]));
    }
    return prevOutput;
}

void external_command(int argc, char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execvp(args[0], args) == -1)
        {
            perror("command eroor:");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Error forking
        perror("command forking eroor:");
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