#include "include/crud.h"

// #include <sys/types.h>
//open file
FILE *openFile(int argc, char **argv)
{
    FILE *fp;
    switch (argc)
    {
        case 1:
            fp = fopen("newDataFile.txt", "w+");
            printf("created new file as newDataFile\n");
            break;
        case 2:
            fp = fopen(argv[1], "r+");
            //if not present then create it
            if (fp == NULL)
                fp = fopen(argv[1], "w+");
            break;
        case 3:
            // create new string with file address
            int size = strlen(argv[1]) + strlen(argv[2]);
            char *path = (char *)malloc(size);
            strcpy(path, argv[2]);
            strcat(path, argv[1]);
            fp = fopen(path, "r+");
            if (fp == NULL)
                fp = fopen(path, "w+");
            free(path);
            break;
        default:
            printError("Invalid Argument");
            exit(EXIT_FAILURE);
    }
    //validate if file was loaded successfully or not
    if (fp == NULL){
        printErrorReason("Could not create file");
        exit(EXIT_FAILURE);
    }
    else
        printSuccess("File loaded");
    return fp;
}

//utils
int mystrcmp(char str1[], char str2[])
{
    int i = 0;
    while (str1[i] != 0 && str2[i] != 0 && str1[i] != '\n' && str2[i] != '\n')
    {
        if (str1[i] != str2[i])
            break;
        i++;
    }
    if (str1[i] == 0 && str2[i] == 0 || str1[i] == '\n' && str2[i] == 0 || str1[i] == '\0' && str2[i] == '\n')
        return 0;
    else
        return -1;
}
Buffer doubleTheSize(Buffer buffer, int *size)
{
    *size = *size * 2;
    return realloc(buffer, *size * sizeof(WordNode *));
}
WordNode *createWordNodeList(char str[])
{
    int wordIndex = 0, lineIndex = 0, size = strlen(str);
    WordNode *head;
    WordNode *node = (WordNode *)calloc(1, sizeof(WordNode));
    head = node;
    node->previous = NULL;
    node->next = NULL;
    WordNode *temp = NULL;
    while (lineIndex < size)
    {
        node->word[wordIndex++] = str[lineIndex++];
        // node->word[wordIndex] = '\0';
        if (str[lineIndex] == ' ')
        {
            temp = (WordNode *)calloc(1, sizeof(WordNode));
            node->next = temp;
            temp->previous = node;
            temp->next = NULL;
            node = temp;
            wordIndex = 0;
            lineIndex++;
        }
    }

    return head;
}
TaskNode *createTaskNode(int op, Cursor cur, char afterBefore, char data[])
{
    TaskNode *taskNode = (TaskNode *)calloc(1, sizeof(TaskNode));
    taskNode->operationCode = op;
    taskNode->cur = cur;
    taskNode->afterBefore = afterBefore;
    strcpy(taskNode->data, data);
    return taskNode;
}
void createTaskNodeAndAttach(int op, Cursor cur, char afterBefore, char data[], TaskNode **head)
{
    TaskNode *taskNode = (TaskNode *)calloc(1, sizeof(TaskNode));
    taskNode->operationCode = op;
    taskNode->cur = cur;
    taskNode->afterBefore = afterBefore;
    strcpy(taskNode->data, data);
    // attach
    taskNode->next = *head;
    *head = taskNode;
    // return taskNode;
}

//used for loading file to buffer
Buffer readFile(FILE *fp, int *lineNo, int *size)
{
    Buffer buffer = (Buffer)calloc(*size, sizeof(WordNode *));
    char lineBuffer[LINESIZE];
    // int result;
    *lineNo = 0;
    // do
    // {
    //     // result = fscanf(fp, "[^\n]", lineBuffer);
    //     WordNode *p = createWordNodeList(lineBuffer);
    //     arr[lineNo++] = p;
    //     // have to double the array size if array is full
    // } while (result != EOF);
    while (fgets(lineBuffer, LINESIZE, fp) != NULL)
    {
        WordNode *p = createWordNodeList(lineBuffer);
        buffer[(*lineNo)++] = p;
        if (*lineNo >= *size)
        {
            buffer = doubleTheSize(buffer, size);
        }
    }
    return buffer;
}
int save(FILE *fp, Buffer buffer, int lineNo){
    WordNode *node;
    for (int i = 0; i < lineNo; i++)
    {   
        node = buffer[i];
        // print words of line
        while (node != NULL)
        {
            fprintf(fp, "%s", node->word);
            node = node->next;
            if(node)
                fprintf(fp, " ");  
        }
    }
    // fclose(fp);
    fflush(fp);
    // SetEndOfFile(fp);
    if(ftruncate(fileno(fp), ftell(fp)) == 0)
        printSuccess("Saved");
    else
        printError("Couldn't save file!");
    fseek(fp,0,SEEK_SET);
}
//search and set cursor
int wordSearch(Buffer buffer, Cursor *cur, char str[], int NoOfLines)
{
    // WordNode* temp = cur->wordPointer;
    WordNode *storedNode = cur->wordPointer;
    //check if file is empty
    if(storedNode == NULL)
        return 0;

    int flag = 0;
    while (1)
    {
        while (cur->wordPointer != NULL)
        {
            // found
            if (mystrcmp(cur->wordPointer->word, str) == 0)
                return 1;
            if (flag && cur->wordPointer == storedNode)
                return 0;
            flag = 1;
            cur->wordPointer = cur->wordPointer->next;
            // not found
        }
        cur->lineIndex++;
        if (cur->lineIndex == NoOfLines)
            cur->lineIndex %= NoOfLines;
        cur->wordPointer = buffer[cur->lineIndex];
    }
}
void setCursor(Buffer buffer, Cursor *cur, int lineNo, int wordNo, int noOfLines)
{
    //validate buffer has data
    if(noOfLines == 0){
        printError("Can't move cursor file is empty!");
        return;
    }
    //set line
    int i = 0, count = 0;
    while(count < lineNo && i < noOfLines){
        if(buffer[i]!=NULL)
            count++;
        i++;
    }
    cur->lineIndex = i-1;

    //set word
    cur->wordPointer = buffer[cur->lineIndex];
    int wordIndex = wordNo - 1;
    while (wordIndex && cur->wordPointer != NULL)
    {
        cur->wordPointer = cur->wordPointer->next;
        wordIndex--;
    }
    if (cur->wordPointer == NULL)
        cur->wordPointer = buffer[cur->lineIndex];
}

// CRUD
/*
   attach functionality for efficient working of undo-redo,
   so insertLine allocates new memory and creates new linked list based on words
   where as attachLine just attach line(already present in memory) to particular position
*/
void print(Buffer buffer, Cursor cursor, int lineNo)
{
    WordNode *node;
    int skipped = 0, isEmpty = 1;
    // printf("\r*********************\n");

    for (int i = 0; i < lineNo; i++)
    {   
        isEmpty = 0;
        node = buffer[i];
        // check if line is present or not
        if (node == NULL)
        {
            skipped++;
            continue;
        }
        printf("\r%d| ", i + 1 - skipped);
        // print words of line
        while (node != NULL)
        {
            if (node == cursor.wordPointer)
            {
                int len = strlen(node->word);
                if (node->word[len - 1] == '\n')
                {
                    node->word[len - 1] = 0;
                    printf("%s%s%s\n", YELB, node->word, RESET);
                    node->word[len - 1] = '\n';
                }
                else
                    printf("%s%s%s", YELB, node->word, RESET);
            }
            else
                printf("%s", node->word);
            node = node->next;
            if(node)
                printf(" ");
        }
        // printf("\n");
    }
    // printf("\r*********************\n");
    if(isEmpty)
        printError("File is Empty");
}
void shiftLines(Buffer buffer, int *noOfLines, int i)
{
    int j = (*noOfLines)++;
    // (*noOfLines)++;

    // have to check for overFlow
    while (j > i)
    {
        buffer[j] = buffer[j - 1];
        j--;
    }
    buffer[i] = NULL;
}
void insertLine(Buffer buffer, int *noOfLines, Cursor *cur, char afterBefore, char str[], TaskNode **undoHead)
{
    strcat(str, "\n");
    WordNode *sentence = createWordNodeList(str);
    // to insert after
    if (afterBefore == 'a' || afterBefore == 'A')
        cur->lineIndex++;
    shiftLines(buffer, noOfLines, cur->lineIndex);
    buffer[cur->lineIndex] = sentence;
    cur->wordPointer = buffer[cur->lineIndex];

    // make entry of delete in undo list
    TaskNode *taskNode = createTaskNode(1, *cur, 0, "");
    taskNode->next = *undoHead;
    *undoHead = taskNode;
}
void attachLine(Buffer buffer, Cursor *cur, TaskNode **head)
{
    buffer[cur->lineIndex] = cur->wordPointer;

    // add to redo list
    TaskNode *taskNode = createTaskNode(1, *cur, 0, "");
    taskNode->next = *head;
    *head = taskNode;
}
void insertWord(Buffer buffer, Cursor *cur, char afterBefore, char str[], TaskNode **undoHead)
{
    WordNode *wordNode = (WordNode *)calloc(1, sizeof(WordNode));
    strcpy(wordNode->word, str);
    // check if cur->wordPointer is null
    if (cur->wordPointer == NULL)
    {
        buffer[cur->lineIndex] = wordNode;
        cur->wordPointer = wordNode;
    }
    else
    {
        if (afterBefore == 'a')
        {
            wordNode->previous = cur->wordPointer;
            wordNode->next = cur->wordPointer->next;
            cur->wordPointer->next = wordNode;
        }
        else
        {
            if (cur->wordPointer->previous != NULL)
            {
                wordNode->previous = cur->wordPointer->previous;
                wordNode->next = cur->wordPointer;
                cur->wordPointer->previous->next = wordNode;
                // cur->wordPointer->previous = wordNode;
                // not last node
            }
            else
            {
                wordNode->previous = NULL;
                wordNode->next = cur->wordPointer;
                buffer[cur->lineIndex] = wordNode;
            }
        }
        if (wordNode->next != NULL)
            wordNode->next->previous = wordNode;
        cur->wordPointer = wordNode;
    }
    createTaskNodeAndAttach(2, *cur, 0, "",undoHead);
    // if (flag)
    // {
    //     // make entry in undo list
    //     TaskNode *taskNode = createTaskNode(2, *cur, 0, "");
    //     taskNode->next = *undoHead;
    //     *undoHead = taskNode;
    // }
}
void attachWord(Buffer buffer, Cursor *cur, char *str, TaskNode **head)
{
    if (cur->wordPointer->previous == NULL && cur->wordPointer->next == NULL)
    {
        buffer[cur->lineIndex] = cur->wordPointer;
    }
    if (cur->wordPointer->previous != NULL)
    {
        cur->wordPointer->previous->next = cur->wordPointer;
    }
    else
        buffer[cur->lineIndex] = cur->wordPointer;
    if (cur->wordPointer->next != NULL)
    {
        cur->wordPointer->next->previous = cur->wordPointer;
    }
    strcpy(cur->wordPointer->word, str);
    // make entry in undo/redo list
    createTaskNodeAndAttach(2, *cur, 0, "", head);
}
void updateWord(Cursor *cur, char str[], TaskNode **head)
{
    if (cur->wordPointer != NULL)
    {
        char *word = cur->wordPointer->word;

        // make entry in undo/redo list
        createTaskNodeAndAttach(3, *cur, 0, word, head);
        // TaskNode *taskNode = createTaskNode(3, *cur, 0, word);

        if (word[strlen(word) - 1] == '\n' && str[strlen(str) - 1] != '\n')
        {
            strcat(str, "\n");
        }

        strcpy(word, str);
    }
    else
        printf("Cursor not pointing!");
}
void deleteLine(Buffer buffer, Cursor *cur, TaskNode **undoHead)
{
    // make entry in undo list
    cur->wordPointer = buffer[cur->lineIndex];
    TaskNode *taskNode = createTaskNode(4, *cur, 0, "");
    taskNode->next = *undoHead;
    *undoHead = taskNode;

    buffer[cur->lineIndex] = NULL;
    while (buffer[++cur->lineIndex] == NULL)
        ;
    cur->wordPointer = buffer[cur->lineIndex];
}
void deleteWord(Buffer buffer, Cursor *cur, TaskNode **undoHead)
{
    if (cur->wordPointer != NULL)
    {
        // save previous word
        char *str = cur->wordPointer->word;

        // top node
        if (cur->wordPointer->previous == NULL)
        {
            createTaskNodeAndAttach(5, *cur, 'b', str, undoHead);

            buffer[cur->lineIndex] = cur->wordPointer->next;
            cur->wordPointer = buffer[cur->lineIndex];

            if (cur->wordPointer != NULL)
                cur->wordPointer->previous = NULL;
            return;
            // cur->wordPointer will save this address in undo stack
        }
        createTaskNodeAndAttach(5, *cur, 'a', str, undoHead);
        cur->wordPointer->previous->next = cur->wordPointer->next;
        // Will only do this when wordPointer is not leaf node
        if (cur->wordPointer->next != NULL)
        {
            cur->wordPointer->next->previous = cur->wordPointer->previous;
        }
        cur->wordPointer = cur->wordPointer->previous;
    }
    else
        printf("Cursor is not set!\n");
}

//undo redo
void undoRedoTask(Buffer buffer, Cursor *cur, TaskNode **head1, TaskNode **head2)
{
    if (*head1 != NULL)
    {
        TaskNode *taskNode = *head1;
        *head1 = (*head1)->next;
        switch (taskNode->operationCode)
        {
        case 1:
            // delete line
            deleteLine(buffer, &(taskNode->cur), head2);
            // *cur = taskNode->cur;
            break;
        case 2:
            // delete word
            deleteWord(buffer, &(taskNode->cur), head2);
            // printf("--%d %p--",taskNode.)
            // *cur = taskNode->cur;
            break;
        case 3:
            // update word
            updateWord(&(taskNode->cur), taskNode->data, head2);

            // *cur = taskNode->cur;
            break;
        case 4:
            // attach line
            attachLine(buffer, &(taskNode->cur), head2);
            break;
        case 5:
            // insert word
            // insertWord(buffer, &(taskNode->cur), taskNode->afterBefore, taskNode->data, head2, 1);
            attachWord(buffer, &(taskNode->cur), taskNode->data, head2);
            break;
        }
        *cur = taskNode->cur;
    }
    else
        printf("stack empty\n");
}
