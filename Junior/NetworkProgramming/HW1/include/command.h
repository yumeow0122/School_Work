#ifndef MAX_COMMAND_SIZE
#define MAX_COMMAND_SIZE 5000
#endif

typedef struct Command
{
    int argc;
    char **args;
} Command;

int command_parse(char *command, Command **commands);

Command **build_command_array();

char *run_command(Command *command, char *prevOut);

void print_command(Command *command);