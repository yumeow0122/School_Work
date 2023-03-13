#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// char *build_printenv_output(int mode, int argc, char **args, char **envp)
// {
//     char *prevOutput = malloc(OUTPUT_SIZE * sizeof(char));
//     memset(prevOutput, 0, OUTPUT_SIZE);
//     if (mode == 0)
//     {
//         for (char **env = envp; *env != NULL; env++)
//         {
//             strcat(prevOutput, *env);
//             strcat(prevOutput, "\n");
//         }
//         prevOutput[strlen(prevOutput) - 1] = '\0';
//     }
//     else if (mode == 1)
//     {
//         strcat(prevOutput, getenv(args[1]));
//     }
//     return prevOutput;
// }

void execute_command(int argc, char **args)
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

// Function where the piped system commands is executed
void pipelinec(char ***cmd, int cmdc)
{
    int fd[2];
    pid_t pid;
    int fdd = 0; /* Backup */

    for (int i = 0; i < cmdc; i++)
    {
        pipe(fd);
        if ((pid = fork()) == -1)
        {
            perror("pipe fork:");
            exit(1);
        }
        else if (pid == 0)
        {
            // child
            dup2(fdd, 0);
            if (i != cmdc - 1)
            {
                dup2(fd[1], 1);
            }
            close(fd[0]);
            execvp((*cmd)[0], *cmd);
            exit(1);
        }
        else
        {
            // parent
            wait(NULL);
            close(fd[1]);
            fdd = fd[0];
            cmd++;
        }
    }
}

// Function where the piped system commands is executed
void pipeline(char ***cmd)
{
    printf("in\n");
    int fd[2];
    pid_t pid;
    int fdd = 0; /* Backup */

    while (*cmd != NULL)
    {
        printf("cur: %s\n", *cmd[0]);
        pipe(fd);
        if ((pid = fork()) == -1)
        {
            perror("pipe fork:");
            exit(1);
        }
        else if (pid == 0)
        {
            // child
            dup2(fdd, 0);
            if (*(cmd + 1) != NULL)
            {
                dup2(fd[1], 1);
            }
            close(fd[0]);
            execvp((*cmd)[0], *cmd);
            exit(1);
        }
        else
        {
            // parent
            wait(NULL);
            close(fd[1]);
            fdd = fd[0];
            cmd++;
        }
    }
    printf("end\n");
}

void single_pipe(char **parsed, char **parsedpipe)
{
    // 0 is read end, 1 is write end
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0)
    {
        printf("\nPipe could not be initialized");
        return;
    }

    p1 = fork();
    if (p1 < 0)
    {
        printf("\nCould not fork");
        return;
    }

    if (p1 == 0)
    {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0], parsed) < 0)
        {
            printf("\nCould not execute command 1..");
            exit(0);
        }
        // Close the pipefd in child 1
        close(pipefd[0]);
        close(pipefd[1]);
    }
    else
    {
        // Parent executing
        p2 = fork();

        if (p2 < 0)
        {
            printf("\nCould not fork");
            return;
        }

        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0)
            {
                printf("\nCould not execute command 2..");
                exit(0);
            }
            // Close the pipefd in child 2
            close(pipefd[0]);
            close(pipefd[1]);
        }
        else
        {
            // parent executing, waiting for two children
            close(pipefd[0]); // Close read end in parent
            close(pipefd[1]); // Close write end in parent
            int status;
            while (wait(&status) > 0)
                ; // Wait for both children to exit
        }
    }
}