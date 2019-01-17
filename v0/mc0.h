#ifndef mc0
#define mc0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <limits.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/resource.h>

void nullTerminateStr(char * str);
void splitByDelim(char *str, char *delim);
void printChildStatistics();

#endif