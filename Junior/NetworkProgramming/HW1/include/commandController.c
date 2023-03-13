#include <stdio.h>
#include <stdlib.h>

#include "./commandJudger.c"
#include "./commandRunner.c"

#ifndef OUTPUT_SIZE
#define OUTPUT_SIZE 3
#endif

#ifndef COMMAND_SIZE
#define COMMAND_SIZE 5000
#endif

void free_data(char ***data, size_t xlen, size_t ylen)
{
    size_t i, j;

    for (i = 0; i < xlen; ++i)
    {
        if (data[i] != NULL)
        {
            for (j = 0; j < ylen; ++j)
                free(data[i][j]);
            free(data[i]);
        }
    }
    free(data);
}

char ***alloc_data(size_t xlen, size_t ylen, size_t zlen)
{
    char ***p;
    size_t i, j;

    if ((p = malloc(xlen * sizeof *p)) == NULL)
    {
        perror("malloc 1");
        return NULL;
    }

    for (i = 0; i < xlen; ++i)
        p[i] = NULL;

    for (i = 0; i < xlen; ++i)
        if ((p[i] = malloc(ylen * sizeof *p[i])) == NULL)
        {
            perror("malloc 2");
            free_data(p, xlen, ylen);
            return NULL;
        }

    for (i = 0; i < xlen; ++i)
        for (j = 0; j < ylen; ++j)
            p[i][j] = NULL;

    for (i = 0; i < xlen; ++i)
        for (j = 0; j < ylen; ++j)
            if ((p[i][j] = malloc(zlen * sizeof *p[i][j])) == NULL)
            {
                perror("malloc 3");
                free_data(p, xlen, ylen);
                return NULL;
            }

    return p;
}

void printCommand(int cnt, char **cmd)
{
    for (int i = 0; i < cnt; i++)
        printf("%s ", cmd[i]);
    printf("\n");
}
void run_command(int argc, char **args, char **envp)
{
    int pipeCount = pipe_count(argc, args);
    if (pipeCount == 0)
    {
        execute_command(argc, args);
        return;
    }

    int notFirst = 1;
    int curCommandIdx = 0, cmdIdx = 0;
    char ***cmd = alloc_data(OUTPUT_SIZE, OUTPUT_SIZE, OUTPUT_SIZE);

    for (int idx = 0; idx < argc; idx++)
    {
        if (strcmp(args[idx], "|") == 0)
        {
            cmd[cmdIdx++][curCommandIdx++] = NULL;
            curCommandIdx = 0;
            continue;
        }
        cmd[cmdIdx][curCommandIdx++] = args[idx];
    }
    cmd[cmdIdx++][curCommandIdx++] = NULL;
    printf("first: %s\n", cmd[0][0]);

    printf("second: %s\n", cmd[1][0]);

    // single_pipe(cmd[0], cmd[1]);
    pipelinec(cmd, cmdIdx);

    char *ls[] = {"ls", "-l", NULL};
    char *rev[] = {"number", NULL};

    char **cmd1[] = {ls, rev, NULL};

    // printf("first: %s\n", cmd1[0][0]);
    // printf("%ld\n", strlen(cmd1[0][1]));

    // printf("first: %s\n", cmd1[0][1]);

    // printf("second: %s\n", cmd1[1][0]);
    // printf("second: %s\n", cmd1[1][1]);

    // printf("%d\n", strcmp(cmd[0][0], cmd1[0][0]));

    // pipeline(cmd1);

    // for (int idx = 0; idx < argc; idx++)
    // {
    //     if (strcmp(args[idx], "|") == 0)
    //     {
    //         curCommand[curCommandIdx++] = NULL;
    //         if (notFirst)
    //         {
    //             notFirst = 0;
    //             for (int i = 0; i < curCommandIdx; i++)
    //                 preCommand[i] = curCommand[i];
    //             preCommandIdx = curCommandIdx;
    //             curCommandIdx = 0;
    //             continue;
    //         }
    //         // printCommand(curCommandIdx, curCommand);
    //         // printCommand(preCommandIdx, preCommand);

    //         for (int i = 0; i < curCommandIdx; i++)
    //             preCommand[i] = curCommand[i];
    //         cmd[cmdIdx++] = curCommand;
    //         preCommandIdx = curCommandIdx;
    //         curCommandIdx = 0;
    //         continue;
    //     }
    //     curCommand[curCommandIdx++] = args[idx];
    // }
    // curCommand[curCommandIdx++] = NULL;

    // printCommand(curCommandIdx, curCommand);
    // printCommand(preCommandIdx, preCommand);
}