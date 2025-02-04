#include "mc2.h"

int IS_PIPED_INPUT; // global variable that stores whether input is from terminal or piped in from file

struct backgroundProcess {
    pid_t pid;
    int numInQueue;
    char commandName[50];
    struct timeval startTime;
};

// This is used to store out background processes when they are executing
struct backgroundProcess RUNNING_BACKGROUND_PROCESSES[MAX_BACKGROUND_PROCESSES];
int numBackgroundProcesses = 0; // the number of background processes currently running

int main(int argc, char **argv) {
    char **comAdd = malloc(MAX_USER_ADDED_COMMANDS); // holds user added commands
    int *comNum = (int *) malloc(sizeof(int)); // number of user added commands
    *comNum = 0;

    if (isatty(fileno(stdin))) { // input is coming from terminal
        IS_PIPED_INPUT = 0;
    } else { // input is coming from a file pipe
        IS_PIPED_INPUT = 1;
    }

    // Run Mid-Day Commander simulation until user exits using (control + c) or "e"
    while (1) {
        if (runMDC(comNum, comAdd)) {
            continue;
        }
    }
}

/**
 * Run the Mid-Day Commander shell simulation
 * @param comNum pointer to the number of user added commands
 * @param comAdd pointer to strings of user added commands
 * @return 0 on success, -1 on failure
 */
int runMDC(int *comNum, char **comAdd) {
    int isBackground = 0; // is the current command to execute a background one?

    // Print message explaining how MDC works to the user
    printInitialMessage(comNum, comAdd);

    // Read in user input
    char userInputStr[BUFF_SIZE];
    char *fullCommand = malloc(BUFF_SIZE); // used to store the custom command that the user enters
    // Check to see if any children processes have completed
    checkChildren();

    char *f = fgets(userInputStr, sizeof (userInputStr - 1), stdin);

    // Check to see if any children processes have completed
    checkChildren();

    // now try to process users command
    if (strlen(userInputStr) > 0) {
        nullTerminateStr(userInputStr);
    }

    if (IS_PIPED_INPUT) {
        if (f == NULL) { // If we detect EOF, prepare to exit
            strcpy(userInputStr, "e");
        }
        printf("%s\n", userInputStr);
    }

    printf("\n");

    // Verify user input
    if (!isValidInput(userInputStr, *comNum)) {
        printf("Invalid input, MDC does not have a command associated with the input \"%s\".\n\n", userInputStr);
        return -1;
    }

    // handle the commands that dont reset on running MDC again
    if (!handlePersistentCommands(userInputStr, comNum, comAdd)) {
        return -1;
    }

    // try to parse the user input to an int since we already validated input
    int userInputInt = strToInt(userInputStr);

    // used to store user input command
    char **args = malloc(BUFF_SIZE * MAX_USER_ADDED_COMMANDS);

    // now process the user input
    if (userInputInt == 0) { // whoami command
        printf("-- Who Am I? --\n");

        args[0] = "whoami";
        args[1] = '\0';
    } else if (userInputInt == 1) { // last command
        printf("-- Last Logins --\n");

        args[0] = "last";
        args[1] = '\0';
    } else if (userInputInt == 2) { // ls command
        printf("-- Directory Listing --\n");

        char argumentBuff[BUFF_SIZE];
        char pathBuff[BUFF_SIZE];

        // get arguments from user
        printf("Arguments?: ");

        fgets(argumentBuff, sizeof argumentBuff, stdin);
        nullTerminateStr(argumentBuff);
        if (IS_PIPED_INPUT) {
            printf("%s\n", argumentBuff);
        }

        // get path from user
        printf("Path?: ");

        fgets(pathBuff, sizeof pathBuff, stdin);
        nullTerminateStr(pathBuff);
        if (IS_PIPED_INPUT) {
            printf("%s\n", pathBuff);
        }
        printf("\n");

        if (strlen(argumentBuff) == 0 && strlen(pathBuff) == 0) { // user specified no args or path
            args[0] = "ls";
            args[1] = '\0';
        } else if (strlen(argumentBuff) == 0) { // user specified only a path
            args[0] = "ls";
            args[1] = pathBuff;
            args[2] = '\0';
        } else if (strlen(pathBuff) == 0) { // user specified only arg(s)
            char buff[BUFF_SIZE];
            strcpy(buff, "ls ");
            strcat(buff, argumentBuff);

            char *buffP = malloc(BUFF_SIZE);
            strcpy(buffP, buff);

            splitInputArgs(args, buffP);
        } else { // user specified both arg(s) and a path
            char buff[BUFF_SIZE];

            strcpy(buff, "ls ");
            strcat(buff, argumentBuff);
            strcat(buff, " ");
            strcat(buff, pathBuff);

            char *buffP = malloc(BUFF_SIZE);
            strcpy(buffP, buff);

            splitInputArgs(args, buffP);
        }
    } else if (userInputInt < *comNum + INITIAL_NUMBER_OF_COMMANDS) { // user added command

        strcpy(fullCommand, comAdd[userInputInt - INITIAL_NUMBER_OF_COMMANDS]);

        // check if it is a background task
        if(fullCommand[strlen(fullCommand) - 1] == '&') {
            nullTerminateStr(fullCommand); // to chop off the '&'
            isBackground = 1;
        }

        splitInputArgs(args, fullCommand);
    }


    // Depending on the input, run the command in the background or foreground
    if(isBackground) {
        char *temp = malloc(BUFF_SIZE);
        strcpy(temp, comAdd[userInputInt - INITIAL_NUMBER_OF_COMMANDS]);

        runBackground(args, temp);

        // print statistics about that command
        printf("-- Command: %s --\n", comAdd[userInputInt - INITIAL_NUMBER_OF_COMMANDS]);
        printf("[%d] %d\n\n", RUNNING_BACKGROUND_PROCESSES[numBackgroundProcesses - 1].numInQueue, RUNNING_BACKGROUND_PROCESSES[numBackgroundProcesses - 1].pid);

    } else{
        runForeground(args);
    }

    return 0;
}


/**
 * This function will be used to determine if any children have finished execution.
 * If they have, then it prints out the statistics for the finished children.
 */
void checkChildren() {
    while(1) {
        struct rusage stats;

        pid_t wpid = wait3(NULL, WNOHANG, &stats);


        if(wpid == -1) {
            // error, no children (probably)
            break;
        } else if (wpid == 0) {
            // children exist, no return yet
            break;
        }
        if (wpid > 0) {

            // end timer
            struct timeval t2;
            gettimeofday(&t2, NULL);

            double elapsedTime;

            int index = 0;

            // try to get the index that this process is in the queue
            for(int i = 0; i < numBackgroundProcesses; i++) {
                if(RUNNING_BACKGROUND_PROCESSES[i].pid == wpid) {
                    index = i;
                }
            }

            // print stats about the job that just finished
            printf("-- Job Complete [%d] --\n", RUNNING_BACKGROUND_PROCESSES[index].numInQueue);
            printf("Process ID: %d\n", wpid);
            printf("[ Output ]\n");

            // compute elapsed time in ms
            elapsedTime = (t2.tv_sec - RUNNING_BACKGROUND_PROCESSES[index].startTime.tv_sec) * 1000.0;      // sec to ms
            elapsedTime += (t2.tv_usec - RUNNING_BACKGROUND_PROCESSES[index].startTime.tv_usec) / 1000.0;   // tack on the us

            printChildStatistics(elapsedTime, stats.ru_minflt, stats.ru_majflt);

            // now remove it from the array
            int i;
            for(i = index; i < numBackgroundProcesses - 1; i++) {
                RUNNING_BACKGROUND_PROCESSES[i] = RUNNING_BACKGROUND_PROCESSES[i + 1];
            }
            numBackgroundProcesses--;

            continue;
        }
    }
}

/**
 * This function runs a child process and *does not* wait until its completion
 * @param args List of arguments to be executed
 * @param fullCommand The full command that the user wants to run
 */
void runBackground(char **args, char *fullCommand) {
    // fork to create a child process
    pid_t cpid = fork();

    struct timeval t1;
    gettimeofday(&t1, NULL);

    // copy this childs data into our array
    numBackgroundProcesses++;
    strcpy(RUNNING_BACKGROUND_PROCESSES[numBackgroundProcesses - 1].commandName, fullCommand);
    RUNNING_BACKGROUND_PROCESSES[numBackgroundProcesses - 1].pid = cpid;
    RUNNING_BACKGROUND_PROCESSES[numBackgroundProcesses - 1].startTime = t1;
    RUNNING_BACKGROUND_PROCESSES[numBackgroundProcesses - 1].numInQueue = numBackgroundProcesses;

    if (cpid > 0) {
        wait3(NULL, WNOHANG, NULL);
    } else if (cpid == 0) {

        // try to execute the command
        if (execvp(args[0], args) == -1) {
            perror("error in execvp()");
        }
    } else {
        printf("Error, failed to fork\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * This function runs a child process and waits until its completion
 * @param args List of arguments to be executed
 */
void runForeground(char **args) {
    // fork to create a child process
    pid_t cpid = fork();
    if (cpid > 0) {
        handleParentProcess(cpid);
    } else if (cpid == 0) {
        // try to execute the command
        if (execvp(args[0], args) == -1) {
            perror("error in execvp()");
        }
    } else {
        printf("Error, failed to fork\n");
        exit(EXIT_FAILURE);
    }
}


/**
 * Handle the parent process
 * @param cpid Process ID of the child
 */
void handleParentProcess(pid_t cpid) {

    // used to calculate elapsed time of child process
    struct timeval t1, t2;
    double elapsedTime;

    // start timer
    gettimeofday(&t1, NULL);

    // track stats before child has returned
    struct rusage before;
    getrusage(RUSAGE_CHILDREN, &before);

    // wait for child to return
    waitpid(cpid, NULL, 0);

    // track stats after waiting for child
    struct rusage after;
    getrusage(RUSAGE_CHILDREN, &after);

    // child has returned, so stop the timer
    gettimeofday(&t2, NULL);

    // compute elapsed time in ms
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // tack on the us


    // get some statistics about the page faults during execution
    long ru_minflt = after.ru_minflt - before.ru_minflt;
    long ru_majflt = after.ru_majflt - before.ru_majflt;

    // print statistics about child process execution
    printChildStatistics(elapsedTime, ru_minflt, ru_majflt);
}

/**
 * Prints the initial message to the user explaining how the MDC works
 * @param comNum pointer to the number of user added commands
 * @param comAdd pointer to strings of user added commands
 * @return void, but prints to stdout
 */
void printInitialMessage(int *comNum, char **comAdd) {

    // Print initial message/options
    printf("===== Mid-Day Commander, v2 =====\n");
    printf("G’day, Commander! What command would you like to run?\n"
           "   0. whoami  : Prints out the result of the whoamicommand\n"
           "   1. last    : Prints out the result of the last command\n"
           "   2. ls      : Prints out the result of a listing on a user-specified path\n");

    // Print user added command(s)
    for (int i = 0; i < *comNum; i++) {
        printf("   %d. %s : User added command\n", i + INITIAL_NUMBER_OF_COMMANDS, comAdd[i]);
    }

    // Print the rest of the users options
    printf("   a. add command : Adds a new command to the menu.\n"
           "   c. change directory : Changes process working directory\n"
           "   e. exit : Leave Mid-Day Commander\n"
           "   p. pwd : Prints working directory\n"
           "   r. running processes : Print list of running processes\n");
    printf("Option?: ");
}

/**
 * Given an input string from the user, this function determines if MDC is capable
 * of processing that input
 * @param userInputString The string with user input
 * @param comNum Number of user added commands
 * @return 1 if MDC can handle user input, 0 otherwise
 */
int isValidInput(char *userInputString, int comNum) {

    // First check the case that input is a string
    if (!strcmp(userInputString, "a") || !strcmp(userInputString, "c") || !strcmp(userInputString, "e") ||
        !strcmp(userInputString, "p") || !strcmp(userInputString, "r")) {
        return 1;
    }

    // The user may have entered a number - lets check this case
    int userInputInt = strToInt(userInputString);
    if (userInputInt == INT_MAX) { // error occurred in conversion
        return 0;
    }

    if (comNum == 0) { // user hasn't added any commands
        if (userInputInt == 0 || userInputInt == 1 || userInputInt == 2) { // the default values a user can enter
            return 1;
        }
    } else if (comNum > 0) { // user has added command(s)
        if (userInputInt < comNum + INITIAL_NUMBER_OF_COMMANDS) {
            return 1;
        }
    }

    return 0;
}

/**
 * Handle the commands that are available to users on running MDC again
 * @param userInputStr  The string with user input
 * @param comNum pointer to number of user added commands
 * @param comAdd pointer to strings of user added commands
 * @return 0 on success, -1 on error
 */
int handlePersistentCommands(char *userInputStr, int *comNum, char **comAdd) {
    if (!strcmp(userInputStr, "a")) {
        printf("-- Add a command --\n");
        printf("Command to add?: ");

        char comBuff[BUFF_SIZE];

        fgets(comBuff, sizeof(comBuff), stdin);

        nullTerminateStr(comBuff);
        if (IS_PIPED_INPUT) {
            printf("%s\n", comBuff);
        }


        if (*comNum == MAX_USER_ADDED_COMMANDS) {
            printf("Cannot add that command, MDC supports up to %d commands\n\n", MAX_USER_ADDED_COMMANDS);
        } else {
            char *comBuffP = malloc(BUFF_SIZE * sizeof(char));
            strcpy(comBuffP, comBuff);
            (*comNum)++;

            comAdd[*comNum - 1] = comBuffP;

            printf("Okay, added with ID %d!\n\n", *comNum + 2);
        }

        return 0;
    } else if (!strcmp(userInputStr, "c")) {
        printf("-- Change Directory --\n");
        printf("New Directory?: ");

        char pathBuff[BUFF_SIZE];

        fgets(pathBuff, sizeof pathBuff, stdin);

        nullTerminateStr(pathBuff);
        if (IS_PIPED_INPUT) {
            printf("%s\n", pathBuff);
        }


        // try to change directory
        if (chdir(pathBuff) == -1) {
            fprintf(stderr, "cd: %s: %s\n", pathBuff, strerror(errno));
        }
        printf("\n");

        return 0;
    } else if (!strcmp(userInputStr, "e")) {

        if (numBackgroundProcesses != 0) {
            // there are processes still running
            printf("MDC can not log out. There are still background processes running.\n\n");
            return 0;
        } else {
            printf("Logging you out, Commander.\n");
            exit(EXIT_SUCCESS);
        }
        return -1;

    } else if (!strcmp(userInputStr, "p")) {
        printf("-- Current Directory --\n");
        char cwd[BUFF_SIZE];

        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd() error");
            return -1;
        } else {
            printf("Directory: %s\n\n", cwd);
        }

        return 0;
    }else if (!strcmp(userInputStr, "r")) {

        printf("-- Background Processes --\n");
        for (int i = 0; i < numBackgroundProcesses; i++) {
            printf("Background Process Number: [%d] \n", RUNNING_BACKGROUND_PROCESSES[i].numInQueue);
            printf("Process ID: %d \n", RUNNING_BACKGROUND_PROCESSES[i].pid);
            printf("Command Name: %s\n", RUNNING_BACKGROUND_PROCESSES[i].commandName);
            printf("\n");

        }
        printf("\n");
        return 0;
    }


    return -1;
}


/**
 * Print statistics about child execution including elapsed time and page faults
 * @param elapsedTime Elapsed time that the MDC shell has been running in milliseconds
 * @param ru_minflt The number of ru_minflt for the child
 * @param ru_majflt The number of ru_majflt for the child
 */
void printChildStatistics(double elapsedTime, long ru_minflt, long ru_majflt) {
    printf("\n");
    printf("-- Statistics ---\n");
    printf("Elapsed Time: %.2lf milliseconds\n", elapsedTime);
    printf("Page Faults: %lu\n", ru_minflt);
    printf("Page Faults (reclaimed): %lu\n", ru_majflt);
    printf("\n");
}

/**
 * Replaces the last character in str with '\0' to terminate the string
 * @param str
 * @return void, but str has been modified
 */
void nullTerminateStr(char *str) {
    int length = strlen(str);

    if (length > 0) {
        str[length - 1] = '\0';
    }
}

/**
 * Parses the string to an int
 * @param str The string to parse to an int
 * @return the string as an int on success, or INT_MAX on failure
 */
int strToInt(char *str) {
    int i = INT_MAX;
    char *end = NULL;
    errno = 0;

    long temp = strtol(str, &end, 10);

    if (end != str && errno != ERANGE && temp >= INT_MIN && temp <= INT_MAX) {
        i = (int) temp;
    }

    return i;
}

/**
 * Split the arguments in str by a " " delimeter, then store that in splitStr
 * @param splitStr
 * @param str
 * @return
 */
int splitInputArgs(char **splitStr, char *str) {
    int i = 0;

    if (strlen(str) != 0) {
        char *delim = " ";
        splitStr[i++] = strtok(str, delim);
        while ((splitStr[i] = strtok(NULL, delim))) {
            i++;
        }
    }
    splitStr[i] = '\0';
    return 0;
}