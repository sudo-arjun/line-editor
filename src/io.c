//module for all custom input/output functions
#include "include/io.h"

void printError(char str[]){
    fprintf(stderr,"%s%s%s\n",ERR, str, RESET);
}
void printErrorReason(char str[]){
    fprintf(stderr,"%s%s\n=> %s %s\n",ERR, str, strerror(errno), RESET);
}
void printSuccess(char str[]){
    printf("%s%s%s\n", SUCCESS, str, RESET);
}
void printHorizontalLine(char designChar){
    int columns;
    #ifdef LINUX
        struct winsize consoleScreen;
        // terminal i/o control get window size - request code to get console size
        ioctl(STDOUT_FILENO,TIOCGWINSZ, &consoleScreen);
        columns = consoleScreen.ws_col;
    #else
        CONSOLE_SCREEN_BUFFER_INFO consoleScreen;
        // first argument is for giving reference to the terminal window
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreen);
        columns = consoleScreen.dwSize.X;
    #endif
    
    for(int i = 0; i < columns; i++){
        printf("%c",designChar);
    }
}
