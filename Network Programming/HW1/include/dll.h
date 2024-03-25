#ifndef DOUBLELINK_H_INCLUDED
#define DOUBLELINK_H_INCLUDED

typedef struct node
{
    struct node *prev;
    struct node *next;
    char *prevCommandOutput;
    int pipeCnt;
} dllNode_t;

dllNode_t *DLL_init();
dllNode_t *node_arg_init(int pipeCnt, char *prevCommandOutput);
void have_pipe_element(dllNode_t *head, char *cmdOut);
void decrement_all_pipe(dllNode_t *head);
void print(dllNode_t *head);

int DLL_isEmpty(const dllNode_t *head);
dllNode_t *DLL_next_node(const dllNode_t *node);
dllNode_t *DLL_prev_node(const dllNode_t *node);
unsigned int DLL_num_nodes(const dllNode_t *head);
void DLL_add_first(dllNode_t *new_node, dllNode_t *head);
void DLL_add_tail(dllNode_t *new_node, dllNode_t *head);
void DLL_addto_prev(dllNode_t *new_node, dllNode_t *node);
void DLL_addto_next(dllNode_t *new_node, dllNode_t *node);
void DLL_delete(dllNode_t *node);
dllNode_t *DLL_concate(dllNode_t *srcList, dllNode_t *dstList);
#endif // DOUBLELINK_H_INCLUDED
