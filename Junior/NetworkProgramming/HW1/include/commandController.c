#include <stdio.h>
#include <stdlib.h>

#include "./commandJudger.c"
#include "./commandRunner.c"

#ifndef OUTPUT_SIZE
#define OUTPUT_SIZE 500
#endif

#ifndef COMMAND_SIZE
#define COMMAND_SIZE 5000
#endif

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
    multi_pipe(cmd, cmdIdx);
    free(cmd);
}