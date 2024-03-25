#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void start_shell()
{
    printf("----- Shell start, enter \"exit\" to leave -----\n");
    setenv("PATH", "/bin:./bin", 1);

}

void end_shell()
{
    printf("----- Shell end. -----\n");
}