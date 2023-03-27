#ifndef MAX_COMMAND_SIZE
#define MAX_COMMAND_SIZE 500
#endif

typedef struct Command
{
    int argc;
    char **args;
} Command;

int command_parse(char *command, Command **commands);

Command **build_command_array();