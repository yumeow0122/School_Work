#ifndef MESSAGE_CONTROLLER_H_INCLUDED
#define MESSAGE_CONTROLLER_H_INCLUDED

#define MSG_SIZE 20

typedef struct Msgbuf
{
    int mtype;
    char context[MSG_SIZE];
} Msgbuf;

void xor(Msgbuf * msg, char *id);
#endif // MESSAGE_CONTROLLER_H_INCLUDED
