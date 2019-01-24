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


#define BUFF_SIZE 12
#define INIT_COM_NUM 2
#define MAX_PARAM 32
void nullTerminateStr(char *str);

void splitByDelim(char *str, char *delim);

void printChildStatistics(double elapsedTime);

int runMDC();

int isValidInput(int userInput, int comNum);
#endif
