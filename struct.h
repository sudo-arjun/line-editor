#define LINESIZE 1000

// structures

typedef struct wordNode
{
    struct wordNode *previous;
    char word[256];
    struct wordNode *next;
} WordNode;

typedef WordNode** Buffer;

typedef struct cursor
{
    int lineIndex;
    WordNode *wordPointer;

} Cursor;

typedef struct taskNode
{
    int operationCode;
    Cursor cur;
    char afterBefore;
    char data[LINESIZE];
    struct taskNode *next;
} TaskNode;

