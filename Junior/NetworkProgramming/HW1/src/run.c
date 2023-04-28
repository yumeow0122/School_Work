#include "run.h"

void run_setenv(Command *command)
{
    if (command->argc != 3)
    {
        printf("setenv error: invalid argument.\n");
        return;
    }
    setenv(command->args[1], command->args[2], 1);
}