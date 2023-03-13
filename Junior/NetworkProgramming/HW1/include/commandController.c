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

int numberPipe = -1;

void run_command(int argc, char **args, char **envp, char **preArgs)
{
    int pipeCount = pipe_count(argc, args);

    int judgePipe = number_pipe(argc, args);
    numberPipe = (numberPipe <= -1 || judgePipe > 0) ? judgePipe : numberPipe;
    if (judgePipe != -1)
    {
        printf("store command\n");
        freeArgs(preArgs);
        copyArgs(argc, args, preArgs);
        return;
    }

    if (pipeCount == 0)
    {
        int statusCode = execute_command(argc, args);
        if(statusCode == 0)
            numberPipe++;
        return;
    }

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

/*
ls -al |2
pre: ls -al |2

ls

number
pre: ls -al | number
cmd:

ls -al
number

*/