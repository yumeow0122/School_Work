#include <stdio.h>
#include <stdlib.h>

#include "./commandJudger.c"
#include "./commandRunner.c"

#ifndef OUTPUT_SIZE
#define OUTPUT_SIZE 50000
#endif

void run_command(int argc, char **args, char **envp)
{
    int curIdx = 0;
    char *prevOutput;

    if (strcmp(args[0], "printenv") == 0)
    {
        // mode is '-1' when command is invalid
        int mode = get_printenv_mode(argc, args);
        if (mode == -1)
        {
            printf("printenv error: too many argument.\n");
            return;
        }
        else if (mode == 0)
            curIdx += 2;
        else if (mode == 1)
            curIdx += 3;

        prevOutput = build_printenv_output(mode, argc, args, envp);
    }
}