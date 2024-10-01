#define LINUX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

#ifdef LINUX
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <errno.h>
#else
    #include <windows.h>
    #include <io.h>
    #define ftruncate _chsize
    #define fileno _fileno
#endif
