#include "mc1.h"

int main(int argc, char **argv) {
    char **comAdd = malloc(MAX_USER_ADDED_COMMANDS *
                           sizeof(char *)); // Array of strings of user added commands: comAdd[0] will be the first user added command with id 3.
    int *comNum = (int *) malloc(sizeof(int)); // number of user added commands
    *comNum = 0;

    int *ru_minflt = (int *) malloc(sizeof(int));
    *ru_minflt = 0;

    int *ru_majflt = (int *) malloc(sizeof(int));
    *ru_majflt = 0;

    // Run Mid-Day Commander simulation until user exits using (control + c)
    while (1) {
        if (runMDC(comNum, comAdd, ru_minflt, ru_majflt)) {
            break;
        }
    }

    return EXIT_FAILURE;
}

/**
 * Run the Mid-Day Commander shell simulation
 * @param comNum pointer to the number of user added commands
 * @param comAdd pointer to strings of user added commands
 * @param ru_minflt pointer to the total number of ru_minflt in the child process
 * @param ru_majflt pointer to the total number of ru_majflt in the child process
 * @return 0 on success
 */
int runMDC(int *comNum, char **comAdd, int *ru_minflt, int *ru_majflt) {

    // Print message explaining how MDC works to the user
    printInitialMessage(comNum, comAdd);

    // Read in user input
    char userInputStr[BUFF_SIZE];
    fgets(userInputStr, sizeof userInputStr, stdin);
    nullTerminateStr(userInputStr);
    printf("\n");

    // Verify user input
    if (!isValidInput(userInputStr, *comNum)) {
        printf("Invalid input, MDC does not have a command associated with the input \"%s\".\n", userInputStr);
        exit(EXIT_FAILURE);
    }

    // handle the commands that dont reset on running MDC again
    if (!handlePersistentCommands(userInputStr, comNum, comAdd)) {
        return 0;
    }

    // fork to create a child process
    pid_t cpid = fork();

    if (cpid > 0) {
        handleParentProcess(cpid, ru_minflt, ru_majflt);
    } else if (cpid == 0) {
        handleChildProcess(userInputStr, *comNum, comAdd);
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
    if (userInputInt == -1 && errno == ERANGE) {
        perror("Error parsing int");
        exit(EXIT_FAILURE);
    }

    if (comNum == 0) { // user hasn't added any commands
        if (userInputInt == 0 || userInputInt == 1 || userInputInt == 2) { // the default values a user can enter
            return 1;
        }
    } else if (userInputInt > 0) { // user has added command(s)
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

        // try to change directory
        if (chdir(pathBuff) == -1) {
            fprintf(stderr, "cd: %s: %s\n", pathBuff, strerror(errno));
//            exit(EXIT_FAILURE);
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

        if (getcwd(cwd, sizeof(cwd)) == NULL)
            perror("getcwd() error");
        else
            printf("Directory: %s\n\n", cwd);

        return 0;
    }

    return -1;
}

/**
 * Handle the parent process
 * @param cpid Process ID of the child
 * @param ru_minflt pointer to the total number of ru_minflt in the child process
 * @param ru_majflt pointer to the total number of ru_majflt in the child process
 */
void handleParentProcess(pid_t cpid, int *ru_minflt, int *ru_majflt) {

    // used to calculate elapsed time of child process
    struct timeval t1, t2;
    double elapsedTime;

    // start timer
    gettimeofday(&t1, NULL);

    // wait for child to return
    waitpid(cpid, NULL, 0);

    // child has returned, so stop the timer
    gettimeofday(&t2, NULL);

    // compute elapsed time in ms
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // tack on the us

    // print statistics about child process execution
    printChildStatistics(elapsedTime, ru_minflt, ru_majflt);
}

/**
 * Handle the child process by processing user input and acting accordingly
 * @param userInputStr The string with user input
 * @param comNum pointer to number of user added commands
 * @param comAdd pointer to strings of user added commands
 */
void handleChildProcess(char *userInputStr, int comNum, char **comAdd) {

    // try to parse the user input to an int
    int userInputInt = strToInt(userInputStr);
    if (userInputInt == -1 && errno == ERANGE) {
        perror("Error parsing int");
        exit(EXIT_FAILURE);
    }

    // now process the user input
    if (userInputInt == 0) { // whoami command
        printf("-- Who Am I? --\n");

        char *const args[] = {"./ls", '\0'};
        execv("/usr/bin/whoami", args);

    } else if (userInputInt == 1) { // last command
        printf("-- Last Logins --\n");

        char *const args[] = {"./last", '\0'};
        execv("/usr/bin/last", args);

    } else if (userInputInt == 2) { // ls command
        printf("-- Directory Listing --\n");

        char argumentBuff[BUFF_SIZE];
        char pathBuff[BUFF_SIZE];

        // get arguments from user, then split them
        printf("Arguments?: ");
        fgets(argumentBuff, sizeof argumentBuff, stdin);
        nullTerminateStr(argumentBuff);
        splitByDelim(argumentBuff, " ");

        // get path from user
        printf("Path?: ");
        fgets(pathBuff, sizeof pathBuff, stdin);
        nullTerminateStr(pathBuff);
        printf("\n");

        if (strlen(argumentBuff) == 0 && strlen(pathBuff) == 0) { // user specified no args or path
            char *const args[] = {"./ls", '\0'};
            execv("/bin/ls", args);
        } else if (strlen(argumentBuff) == 0) { // user specified only a path
            char *const args[] = {"./ls", pathBuff, '\0'};
            execv("/bin/ls", args);
        } else if (strlen(pathBuff) == 0) { // user specified only arg(s)
            char *const args[] = {"./ls", argumentBuff, '\0'};
            execv("/bin/ls", args);
        } else { // user specified both arg(s) and a path
            char *const args[] = {"./ls", argumentBuff, pathBuff, '\0'};
            execv("/bin/ls", args);
        }
    } else if (userInputInt < comNum + INITIAL_NUMBER_OF_COMMANDS) { // user added command

        // the command the user added
        char *userCommand = malloc(sizeof(BUFF_SIZE));
        userCommand = comAdd[userInputInt - INITIAL_NUMBER_OF_COMMANDS];

        char *command = malloc(sizeof(BUFF_SIZE)); // first part of the command
        char *secondPart = malloc(sizeof(BUFF_SIZE)); // everything after the first part, aka the arguments

        char *const sepAt = strchr(userCommand, ' ');
        if (sepAt != (NULL)) {
            *sepAt = '\0'; // overwrite first separator, creating two strings.
            command = userCommand; // first part
            secondPart = (sepAt + 1); // second part (rest of string)
        } else { // no args specified
            command = userCommand;
            secondPart = '\0';
        }

        // split the arguments
        splitByDelim(secondPart, " ");

        // create the executable name
        char executableName[BUFF_SIZE];
        strcpy(executableName, "./");
        strcat(executableName, command);

        // execute the command
        char *const args[] = {executableName, secondPart, '\0'};

        if (execvp(command, args) == -1) {
            perror("error in execvp()");
        }
    }

    exit(1); // execv returned, meaning error
}

/**
 * Print statistics about child execution including elapsed time and page faults
 * @param elapsedTime Elapsed time that the MDC shell has been running in milliseconds
 * @param ru_minflt pointer to the total number of ru_minflt in the child process
 * @param ru_majflt pointer to the total number of ru_majflt in the child process
 * @return void, but prints text to stdout
 */
void printChildStatistics(double elapsedTime, int *ru_minflt, int *ru_majflt) {

    // get usage statistics about child process
    struct rusage childUsage;
    getrusage(RUSAGE_CHILDREN, &childUsage);

    // print out child usage statistics
    printf("\n");
    printf("-- Statistics ---\n");
    printf("Elapsed Time: %.2lf milliseconds\n", elapsedTime);
    printf("Page Faults: %lu\n", childUsage.ru_minflt - *ru_minflt);
    printf("Page Faults (reclaimed): %lu\n", childUsage.ru_majflt - *ru_majflt);
    printf("\n");

    *ru_minflt = childUsage.ru_minflt;
    *ru_majflt = childUsage.ru_majflt;
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
 * Splits the given string by a delimiter
 * @param str The string we want to split
 * @param delim The delimiter used to split the string
 * @return void, but str has been tokenized
 */
void splitByDelim(char *str, char *delim) {
    // Returns first token
    char *token = strtok(str, delim);

    // Keep processing tokens while a delimiter present in the string
    while (token != NULL) {
//        printf("%s\n", token);
        token = strtok(NULL, delim);
    }
}


/**
 * Parses the string to an int
 * @param str The string to parse to an int
 * @return the string as an int on success, or -1 AND errno set to ERANGE on failure
 */
int strToInt(char *str) {
    int i = -1;
    char *end = NULL;
    errno = 0;

    long temp = strtol(str, &end, 10);

    if (end != str && errno != ERANGE && temp >= INT_MIN && temp <= INT_MAX) {
        i = (int) temp;
    }

    return i;
}