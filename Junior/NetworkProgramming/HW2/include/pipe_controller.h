#ifndef PIPE_H_INCLUDED
#define PIPE_H_INCLUDED

#define MAX_USER_ID 10
#define MAX_USER_NAME 10
#define MAX_USER_IP 20

typedef struct PipeData
{
    int pipeCnt;
    char *prevOutput;
} PipeData;

typedef struct Pipe
{
    PipeData *data;
    struct Pipe *prev;
    struct Pipe *next;
} Pipe;

PipeData *pipe_data_init();
Pipe *pipe_init();

/**
 * @brief add pipe to the tail of the list
 * @param head of the list
 * @param npipe new pipe
 * @return void
 **/
void add_pipe(Pipe *head, Pipe *npipe);

/**
 * @brief remove pipe from the list
 * @param head of the list
 * @return void
 **/
void delete_pipe(Pipe *pipe);

/**
 * @brief decremet all pipe in the list
 * @param head of the list
 * @return void
 **/
void decrement_all_pipe(Pipe *head);

/**
 * @brief have pipr command now
 * @param head of the list
 * @param the pipe command output
 * @return void
 **/
void have_pipe_cmd(Pipe *head, char *output);

/**
 * @brief remove unuse pipe in the list
 * @param head of the list
 * @return void
 **/
void remove_unuse_pipe(Pipe *head);

/**
 * @brief log all pipe in the list
 * @param head of the list
 * @return void
 **/
void log_all_pipe(Pipe *head);
#endif // DOUBLELINK_H_INCLUDED
