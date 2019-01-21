#include "mc1.h"

int main(int argc, char **argv) {
    char **comAdd = malloc(
            0); //Array of strings of user added commands: comAdd[0] will be the first user added command with id 3.
    int *comNum = (int *) malloc(sizeof(int)); // number of user added commands
    *comNum = 0;

    // Run Mid-Day Commander simulation until user exits using (control + c)
    while (1) {
        if (runMDC(comNum, comAdd) == -1) {
            break;
        }
    }

    return EXIT_FAILURE;
}

/**
 * Replaces the last character in str with '\0' to terminate the string
 * @param str
 * @return
 */
void nullTerminateStr(char *str) {
    int length = strlen(str);

    if (length > 0) {
        str[length - 1] = '\0';
    }
}

/**
 * Splits the given string by a delimiter
 * @param str
 * @param delim
 */
void splitByDelim(char *str, char *delim) {
    // Returns first token
    char *token = strtok(str, delim);

    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL) {
//        printf("%s\n", token);
        token = strtok(NULL, delim);
    }
}

/**
 * Print statistics about child execution including elapsed time and page faults
 * @param elapsedTime Elapsed time that the MDC shell has been running in milliseconds
 */
void printChildStatistics(double elapsedTime) {

    // get usage statistics about child process
    struct rusage childUsage;
    getrusage(RUSAGE_CHILDREN, &childUsage);

    // print out child usage statistics
    printf("\n");
    printf("-- Statistics ---\n");
    printf("Elapsed Time: %.2lf milliseconds\n", elapsedTime);
    printf("Page Faults: %lu\n", childUsage.ru_minflt);
    printf("Page Faults (reclaimed): %lu\n", childUsage.ru_majflt);
    printf("\n");
}

/**
 * Run the Mid-Day Commander shell simulation
 * @param comNum pointer to the number of user added commands
 * @param comAdd pointer to strings of user added commands
 * @return returns -1 on error
 */
int runMDC(int *comNum, char **comAdd) {
    // Get user input
    printf("===== Mid-Day Commander, v0 =====\n");
    printf("G’day, Commander! What command would you like to run?\n"
           "   0. whoami  : Prints out the result of the whoamicommand\n"
           "   1. last    : Prints out the result of the last command\n"
           "   2. ls      : Prints out the result of a listing on a user-specified path\n");

    // Print user added command
    for (int i = 0; i < *comNum; i++) {
        printf("   %d. %s : User added command\n", i + 3, comAdd[i]);
    }

    printf("   a. add command : Adds a new command to the menu.\n"
           "   c. change directory : Changes process working directory\n"
           "   e. exit : Leave Mid-Day Commander\n"
           "   p. pwd : Prints working directory\n");
    printf("Option?: ");
    char tempBuff[BUFF_SIZE];
    fgets(tempBuff, sizeof tempBuff, stdin);
    nullTerminateStr(tempBuff);
    printf("\n");

    if (!strcmp(tempBuff, "a")) {
        printf("-- Add a command --\n");
        printf("Command to add?:");
        char comBuff[BUFF_SIZE];
        fgets(comBuff, sizeof(comBuff), stdin);
        nullTerminateStr(comBuff);
        char *comBuffP = malloc(BUFF_SIZE * sizeof(char));
        strcpy(comBuffP, comBuff);
        (*comNum)++;
        comAdd = (char **) realloc(comAdd, *comNum * sizeof(char *));
        comAdd[*comNum - 1] = comBuffP;
        return 0;
    } else if (!strcmp(tempBuff, "c")) {
        printf("--change Directory--\n");
        printf("new Directory?:");
        char pathBuff[BUFF_SIZE];
        fgets(pathBuff, sizeof pathBuff, stdin);
        nullTerminateStr(pathBuff);
        chdir(pathBuff);
        printf("\n");
        return 0;
    } else if (!strcmp(tempBuff, "e")) {
        printf("Logging you out, Commander.\n");
        exit(EXIT_SUCCESS);
        return 0;
    } else if (!strcmp(tempBuff, "p")) {
        printf("--Current Directory--\n");
        char *dir = get_current_dir_name();
        printf("%s\n", dir);
        return 0;
    }

    // try to parse user input
    int userInput = -1;
    char *end = NULL;
    errno = 0;

    long temp = strtol(tempBuff, &end, 10);

    if (end != tempBuff && errno != ERANGE && temp >= INT_MIN && temp <= INT_MAX) {
        userInput = (int) temp;
    }

    // verify user input
    if (!isValidInput(userInput)) {
        printf("Invalid input, MDC does not have a command associated with the input \"%s\".\n", tempBuff);
        return -1;
    }

    // fork to create a child process
    pid_t cpid = fork();

    if (cpid > 0) {
        // parent process

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
        printChildStatistics(elapsedTime);
    } else if (cpid == 0) {
        // child process

        if (userInput == 0) { // whoami command
            printf("-- Who Am I? --\n");

            char *const args[] = {"./ls", '\0'};
            execv("/usr/bin/whoami", args);

        } else if (userInput == 1) { // last command
            printf("-- Last Logins --\n");

            char *const args[] = {"./last", '\0'};
            execv("/usr/bin/last", args);

        } else if (userInput == 2) { // ls command
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
        }

        exit(1); // execv returned, meaning error
    } else {
        printf("Error, failed to fork\n");
        return -1;
    }

    return 0;
}

/**
 * This function determines if MDC can support the user requested command
 * @param userInput
 * @return 1 if user input is valid, 0 otherwise
 */
int isValidInput(int userInput) {
    if (userInput == 0 || userInput == 1 || userInput == 2) {
        return 1;
    }

    return 0;
}
