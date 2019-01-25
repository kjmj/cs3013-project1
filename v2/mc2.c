#include "mc2.h"

int IS_PIPED_INPUT; // global variable that stores whether input is from terminal or piped in from file

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

    // Print message explaining how MDC works to the user
    printInitialMessage(comNum, comAdd);

    // Read in user input
    char userInputStr[BUFF_SIZE];

    char *f = fgets(userInputStr, sizeof userInputStr, stdin);

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
        char *fullCommand = malloc(BUFF_SIZE);
        strcpy(fullCommand, comAdd[userInputInt - INITIAL_NUMBER_OF_COMMANDS]);

        splitInputArgs(args, fullCommand);
    }

    // fork to create a child process
    pid_t cpid = fork();

    printf("%d\n", cpid);
    if (cpid > 0) {
        handleParentProcess(cpid);

        // when we get back here, the child has finished execution
    } else if (cpid == 0) {

        printf("executing command\n");
        char *command = args[0];

        // create the executable name
        char executableName[BUFF_SIZE];
        strcpy(executableName, "./");
        strcat(executableName, args[0]);
        args[0] = executableName;

        // try to execute the command
        if (execvp(command, args) == -1) {
            perror("error in execvp()");
        }
    } else {
        printf("Error, failed to fork\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

/**
 * Prints the initial message to the user explaining how the MDC works
 * @param comNum pointer to the number of user added commands
 * @param comAdd pointer to strings of user added commands
 * @return void, but prints to stdout
 */
void printInitialMessage(int *comNum, char **comAdd) {

    // Print initial message/options
    printf("===== Mid-Day Commander, v0 =====\n");
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
           "   p. pwd : Prints working directory\n");
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
        !strcmp(userInputString, "p")) {
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
        printf("Logging you out, Commander.\n");
        exit(EXIT_SUCCESS);

        return 0;
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
    }

    return -1;
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

    // track stats before waiting
    struct rusage before;
    getrusage(RUSAGE_CHILDREN, &before);

    printf("before child return\n");
    // wait for child to return
    while(1) {
//        printf("%d\n", cpid);
        pid_t w = waitpid(-1, NULL, WNOHANG);
//        printf("%d\n", w);

        if(w == 0 ) {
            printf("child hasnt changed states\n");
            // children hasnt changed states
            break;
        } else {
            // child has changed state
            printf("child has changed state, pid of %d\n", w);
            break;
        }
    }
    


    printf("after child return\n");


    /**
     *
     * Either of these functions can be called with the WNOHANG option, which causes the wait() function to
     * not block but rather return with an error code (e.g., “nobody is ready to be waited on yet”).
     *
     * A suggested approach to handling background tasks is as follows.
     *
     * After forking a child process to invoke a background command (i.e., with a ‘&’ character at the end),
     * go into a loop using wait3(WNOHANG) to wait for any child to finish. If wait3() returns information
     * about a child process that has finished, print its statistics and repeat the loop. However, if wait3()
     * indicates that no child process has finished lately, exit the loop and prompt for the next command.
     * In the case that a command is not a background process (i.e., does not end with a ‘&’ character), then
     * you should use a wait3() loop without the WNOHANG argument. This will pick up any previous background
     * commands that may have completed. Once the non-background task has been waited for, loop again using
     * wait3(WNOHANG) to pick up any remaining tasks that have finished. When wait3(WNOHANG) returns with an
     * error, then prompt for the next command.
     */
    // track stats after waiting
    struct rusage after;
    getrusage(RUSAGE_CHILDREN, &after);

    // child has returned, so stop the timer
    gettimeofday(&t2, NULL);

    // compute elapsed time in ms
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // tack on the us

    // print statistics about child process execution
    printChildStatistics(elapsedTime, &before, &after);
}

/**
 * Print statistics about child execution including elapsed time and page faults
 * @param elapsedTime Elapsed time that the MDC shell has been running in milliseconds
 * @param before filled out rusage struct that was created before fork
 * @param after filled out rusage struct that was created after fork
 * @return void, put prints text to stdout
 */
void printChildStatistics(double elapsedTime, struct rusage *before, struct rusage *after) {
    // print out child usage statistics
    printf("\n");
    printf("-- Statistics ---\n");
    printf("Elapsed Time: %.2lf milliseconds\n", elapsedTime);
    printf("Page Faults: %lu\n", (*after).ru_minflt - (*before).ru_minflt);
    printf("Page Faults (reclaimed): %lu\n", (*after).ru_majflt - (*before).ru_majflt);
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