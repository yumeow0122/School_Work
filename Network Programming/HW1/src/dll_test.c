// #include "dll.h"
// #include <stdio.h>
// #include <stdlib.h>

// typedef struct data
// {
//     dllNode_t n;
//     int d;
// } data;

// void print(dllNode_t *head)
// {
//     dllNode_t *j = head -> next;
//     printf("Current link list:{");
//     while(j != head)
//     {
//         printf("%d",((data*)j) -> d);
//         j = j -> next;
//         if(j != head)
//             printf(", ");
//     }
//     printf("}\n");
// }

// void print2(dllNode_t *head)
// {
//     dllNode_t *j = head -> next;
//     printf("{");
//     while(j != head)
//     {
//         printf("%d",((data*)j) -> d);
//         j = j -> next;
//         if(j != head)
//             printf(", ");
//     }
//     printf("}");
// }

// int main()
// {
//     dllNode_t *head = DLL_init();
//     print(head);
//     printf("The link list is empty or not:%d\n", DLL_isEmpty(head));
//     printf("\n");

//     for(int i = 0;i < 10;i ++)  //�[10��node
//     {
//         data *tmp = (data*)malloc(sizeof(data));
//         tmp -> d = i;
//         printf("Insert a node to the first, data = %d\n", i);
//         DLL_add_first((dllNode_t*)tmp, head);
//     }
//     printf("\n");

//     printf("The link list is empty or not:%d\n", DLL_isEmpty(head));
//     printf("There are %u node in the link list.\n", DLL_num_nodes(head));
//     print(head);
//     printf("\n");

//     printf("Insert a node to the last, data = 11\n");
//     data *one = (data*)malloc(sizeof(data));
//     one -> d = 11;
//     DLL_add_tail((dllNode_t*)one, head);
//     print(head);
//     printf("\n");

//     printf("Insert a node after the second node, data = 12\n");
//     one = (data*)malloc(sizeof(data));
//     one -> d = 12;
//     DLL_addto_next((dllNode_t*)one, head -> next -> next);
//     print(head);
//     printf("\n");

//     printf("Insert a node before the second node, data = 13\n");
//     one = (data*)malloc(sizeof(data));
//     one -> d = 13;
//     DLL_addto_prev((dllNode_t*)one, head -> next -> next);
//     print(head);
//     printf("\n");

//     printf("Delete the second node\n");
//     dllNode_t *j = head -> next -> next;
//     DLL_delete(j);
//     free(j);
//     print(head);
//     printf("\n");

//     printf("The previous node of the second node:%d\n", ((data*)DLL_prev_node(head -> next -> next)) -> d);
//     printf("The next node of the second node:%d\n", ((data*)DLL_next_node(head -> next -> next)) -> d);
//     printf("\n");

//     dllNode_t *head2 = DLL_init();
//     for(int i = 0;i < 5;i ++)  //�[10��node
//     {
//         data *tmp = (data*)malloc(sizeof(data));
//         tmp -> d = i;
//         DLL_add_first((dllNode_t*)tmp, head2);
//     }

//     printf("Connect ");
//     print2(head);
//     printf(" and ");
//     print2(head2);
//     printf("\n");
//     head = DLL_concate(head2, head);
//     print(head);

//     return 0;
// }
