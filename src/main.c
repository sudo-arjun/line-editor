#include "include/crud.h"
#define CHECK_BUFFER noOfLines == 0 && cursor.wordPointer == NULL

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
        printHorizontalLine('*');
        printf("1. Print File\n");
        printf("2. Search\n");
        printf("3. Move Cursor\n");
        printf("4. Insert Line\n");
        printf("5. insert Word\n");
        printf("6. Update Word\n");
        printf("7. Delete Line\n");
        printf("8. Delete Word\n");
        printf("9. Undo\n");
        printf("10. Redo\n");
        printf("11. Save\n");
        printf("12. Exit\n");
        printf("Enter Option:");
        scanf("%d", &choice);
        //validate if buffer is empty
        if(CHECK_BUFFER && choice != 4 && choice != 1 && choice != 9 && choice != 10)
        {
            printError("file is empty, perform insert line operation first!");
            continue;
        }
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
                printError("Word not found!");
                // setCursor(buffer, &cursor, 1, 1);
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
            if (innerChoice == 'y' || CHECK_BUFFER)
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
        case 6:
            printf("Enter new word: ");
            scanf(" %s", word);
            updateWord(&cursor, word, &undoHead);
            redoHead = NULL;
            break;
        case 7:
            // delete line
            deleteLine(buffer, &cursor, &undoHead);
            redoHead = NULL;
            break;
        case 8:
            // delete word
            deleteWord(buffer, &cursor, &undoHead);
            redoHead = NULL;
            break;
        case 9:
            // undo
            undoRedoTask(buffer, &cursor, &undoHead, &redoHead);
            print(buffer, cursor, noOfLines);
            break;
        case 10:
            undoRedoTask(buffer, &cursor, &redoHead, &undoHead);
            print(buffer, cursor, noOfLines);
            break;
        case 11:
            printf("in save\n");
            save(fp, buffer, noOfLines);
            break;
        case 12:
            exit(0);
            break;
        default:
            getchar();
            printf("Enter valid choice!");
        }
    }

    return 0;
}
