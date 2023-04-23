#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#define READ_END 0
#define WRITE_END 1

#define MAX_COMMAND_SIZE 1000
#define MAX_INPUT_SIZE 5000
#define MAX_OUTPUT_SIZE 5000

#define die(e)                      \
    do                              \
    {                               \
        fprintf(stderr, "%s\n", e); \
        exit(EXIT_FAILURE);         \
    } while (0);
    
typedef struct Command
{
    int argc;
    char **args;
} Command;

Command **get_command_array();

int command_parse(char *command, Command **commands);

char *run_command(Command *command, char *prevOut);

void print_command(Command *command);
void sigpipe_handler(int signum);
#endif // COMMAND_H_INCLUDED
