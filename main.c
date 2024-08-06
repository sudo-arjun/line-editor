#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crud.h"

int main(int argc, char **argv)
{
    // open file
    FILE *fp = openFile(argc, argv);

    // get file data into Buffer
    int noOfLines, sizeOfBuffer = 25;
    Buffer buffer = readFile(fp, &noOfLines, &sizeOfBuffer);
    Cursor cursor = {0, buffer[cursor.lineIndex]};
    TaskNode *undoHead = NULL, *redoHead;

    int choice, a, b;
    char word[256], innerChoice, sentence[LINESIZE];
    while (1)
    {
        printf("\n1. print file\n");
        printf("2. search\n");
        printf("3. move Cursor\n");
        printf("4. insertLine\n");
        printf("5. insertWord\n");
        // printf("6. updateLine\n");
        printf("7. updateWord\n");
        printf("8. deleteLine\n");
        printf("9. deleteWord\n");
        printf("10. Undo\n");
        printf("11. Redo\n");
        printf("Enter Option:");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            print(buffer, cursor, noOfLines);
            break;
        case 2:
            printf("Enter word to search: ");
            scanf("%s", word);
            int result = wordSearch(buffer, &cursor, word, noOfLines);
            if (result)
            {
                printf("Searched successfully, now cursor at your word\n");
                printf("Want to see file(y/n): ");
                scanf(" %c", &innerChoice);
                if (innerChoice == 'y')
                    print(buffer, cursor, noOfLines);
            }
            else
            {
                printf("word not found after cursor\n");
                // setCursor(buffer, &cursor, 1, 1);
                // printf("Now cursor pointing at first word!");
            }
            break;
        case 3:
            printf("Enter line no. and word no.: ");
            scanf("%d %d", &a, &b);
            setCursor(buffer, &cursor, a, b, noOfLines);
            break;
        case 4:
            printf("Enter a line: ");
            // getchar();
            scanf(" %[^\n]s", sentence);
            printf("Want to insert before(y/n): ");
            scanf(" %c", &innerChoice);
            if (innerChoice == 'y')
                innerChoice = 'b';
            else
                innerChoice = 'a';
            insertLine(buffer, &noOfLines, &cursor, innerChoice, sentence, &undoHead);
            redoHead = NULL;
            break;
        case 5:
            printf("Enter word: ");
            scanf("%s", word);
            printf("Want to insert before(y/n): ");
            scanf(" %c", &innerChoice);
            if (innerChoice == 'y')
                innerChoice = 'b';
            else
                innerChoice = 'a';
            insertWord(buffer, &cursor, innerChoice, word, &undoHead);
            // addToUndoList(buffer,cursor,)
            redoHead = NULL;
            break;
        case 7:
            printf("Enter new word: ");
            scanf(" %s", word);
            updateWord(&cursor, word, &undoHead);
            redoHead = NULL;
            break;
        case 8:
            // delete line
            deleteLine(buffer, &cursor, &undoHead);
            redoHead = NULL;
            break;
        case 9:
            // delete word
            deleteWord(buffer, &cursor, &undoHead);
            redoHead = NULL;
            break;
        case 10:
            // undo
            undoRedoTask(buffer, &cursor, &undoHead, &redoHead);
            print(buffer, cursor, noOfLines);
            break;
        case 11:
            undoRedoTask(buffer, &cursor, &redoHead, &undoHead);
            print(buffer, cursor, noOfLines);
            break;
        default:
            getchar();
            printf("Enter valid choice!");
        }
    }

    return 0;
}
