#include <stdio.h>
#include <stdlib.h>

#ifndef MAX_OUTPUT_SIZE
#define MAX_OUTPUT_SIZE 500
#endif

typedef struct node
{
    struct node *prev;
    struct node *next;
    char *prevCommandOutput;
    int pipeCnt;
} dllNode_t;

dllNode_t *DLL_init()
{
    dllNode_t *newlist = (dllNode_t *)malloc(sizeof(dllNode_t));
    newlist->prevCommandOutput = malloc(MAX_OUTPUT_SIZE * sizeof(char));
    newlist->pipeCnt = 0;
    newlist->prev = newlist;
    newlist->next = newlist;
    return newlist;
}

dllNode_t *node_arg_init(int pipeCnt, char *prevCommandOutput)
{
    dllNode_t *newNode = DLL_init();
    newNode->pipeCnt = pipeCnt;
    newNode->prevCommandOutput = prevCommandOutput;
    return newNode;
}

int DLL_isEmpty(const dllNode_t *head)
{
    return head->next == head;
}

dllNode_t *DLL_next_node(const dllNode_t *node)
{
    return node->next;
}

dllNode_t *DLL_prev_node(const dllNode_t *node)
{
    return node->prev;
}

unsigned int DLL_num_nodes(dllNode_t *head)
{
    dllNode_t *j = head;
    unsigned int counts = 0;
    while (j->next != head)
    {
        counts++;
        j = j->next;
    }
    return counts;
}

void DLL_add_first(dllNode_t *new_node, dllNode_t *head)
{
    new_node->next = head->next;
    new_node->prev = head;
    head->next->prev = new_node;
    head->next = new_node;
}

void DLL_add_tail(dllNode_t *new_node, dllNode_t *head)
{
    new_node->next = head;
    new_node->prev = head->prev;
    head->prev->next = new_node;
    head->prev = new_node;
}

void DLL_addto_prev(dllNode_t *new_node, dllNode_t *node)
{
    new_node->next = node;
    new_node->prev = node->prev;
    node->prev->next = new_node;
    node->prev = new_node;
}

void DLL_addto_next(dllNode_t *new_node, dllNode_t *node)
{
    new_node->next = node->next;
    new_node->prev = node;
    node->next->prev = new_node;
    node->next = new_node;
}

void DLL_delete(dllNode_t *node) //(�qnode�Ҧb�� Linked List ���R�����I)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

dllNode_t *DLL_concate(dllNode_t *srcList, dllNode_t *dstList) //(�NsrcList ��bdstList����)
{
    dstList->prev->next = srcList->next;
    srcList->next->prev = dstList->prev;
    srcList->prev->next = dstList;
    dstList->prev = srcList->prev;
    return dstList;
}
