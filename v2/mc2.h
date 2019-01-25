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

#include <stdio_ext.h>


#define BUFF_SIZE 256
#define INITIAL_NUMBER_OF_COMMANDS 3
#define MAX_USER_ADDED_COMMANDS 50
#define MAX_BACKGROUND_PROCESSES 100

int runMDC(int *comNum, char **comAdd);

void printInitialMessage(int *comNum, char **comAdd);

int isValidInput(char *userInputString, int comNum);

int handlePersistentCommands(char *userInputStr, int *comNum, char **comAdd);

void handleParentProcess(pid_t cpid);

//void printChildStatistics(double elapsedTime, struct rusage *before, struct rusage *after);
void printChildStatistics(double elapsedTime, long ru_minflt, long ru_majflt);

void nullTerminateStr(char *str);

int strToInt(char *str);

int splitInputArgs(char **splitStr, char *str);



void runForeground(char **args);
void runBackground(char **args);
void checkChildren();

#endif
