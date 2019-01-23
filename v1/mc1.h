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
#define MAX_USER_ADDED_COMMANDS 50

int runMDC(int *comNum, char **comAdd, int *ru_minflt, int *ru_majflt);

void printInitialMessage(int *comNum, char **comAdd);

int isValidInput(char *userInputString, int comNum);

int handlePersistentCommands(char *userInputStr, int *comNum, char **comAdd);

void handleParentProcess(pid_t cpid, int *ru_minflt, int *ru_majflt);

void handleChildProcess(char *userInputStr, int comNum, char **comAdd);

void printChildStatistics(double elapsedTime, int *ru_minflt, int *ru_majflt);

void nullTerminateStr(char *str);

void splitByDelim(char *str, char *delim);

int strToInt(char *str);

#endif
