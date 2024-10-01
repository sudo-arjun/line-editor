#include "io.h"

void printHorizontalLine(char designChar);

FILE *openFile(int argc, char **argv);
Buffer doubleTheSize(Buffer buffer, int *size);
WordNode *createWordNodeList(char str[]);
Buffer readFile(FILE *fp, int *lineNo, int *size);
void print(Buffer buffer, Cursor cursor, int lineNo);
int mystrcmp(char str1[], char str2[]);
int wordSearch(Buffer buffer, Cursor *cur, char str[], int NoOfLines);
void setCursor(Buffer buffer, Cursor *cur, int lineNo, int wordNo, int noOfLines);
int save(FILE *fp, Buffer buffer, int lineNo);

// CRUD

TaskNode *createTaskNode(int op, Cursor cur, char afterBefore, char data[]);
void shiftLines(Buffer buffer, int *noOfLines, int i);
void insertLine(Buffer buffer, int *noOfLines, Cursor *cur, char afterBefore, char str[], TaskNode **undoHead);
void attachLine(Buffer buffer, Cursor *cur, TaskNode **head);
void insertWord(Buffer buffer, Cursor *cur, char afterBefore, char str[], TaskNode **undoHead);
void updateWord(Cursor *cur, char str[], TaskNode **undoHead);
void deleteLine(Buffer buffer, Cursor *cur, TaskNode **undoHead);
void deleteWord(Buffer buffer, Cursor *cur, TaskNode **undoHead);
void undoRedoTask(Buffer buffer, Cursor *cur, TaskNode **head1, TaskNode **head2);