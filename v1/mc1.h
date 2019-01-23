#ifndef mc1
#define mc1

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


#define BUFF_SIZE 256
#define INITIAL_NUMBER_OF_COMMANDS 3
#define MAX_ARGS 50

void nullTerminateStr(char *str);

void splitByDelim(char *str, char *delim);

void printChildStatistics(double elapsedTime);

int runMDC(int *comNum, char **comAdd);

int isValidInput(int userInput, int comNum);

#endif
