#include "mc0.h"

int main(int argc, char **argv) {

    // Run Mid-Day Commander simulation until user exits
    while (1) {
        if (runMDC() == 1) {
            return 1;
        }
    }
}

/**
 * Replaces the last character in str to null terminate the string
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
 * @param elapsedTime
 */
void printChildStatistics(double elapsedTime) {

    struct rusage childUsage;
    getrusage(RUSAGE_CHILDREN, &childUsage);

    printf("\n");
    printf("-- Statistics ---\n");
    printf("Elapsed Time: %.2lf milliseconds\n", elapsedTime);
    printf("Page Faults: %lu\n", childUsage.ru_minflt);
    printf("Page Faults (reclaimed): %lu\n", childUsage.ru_majflt);
    printf("\n");
}

/**
 * Run the Mid-Day Commander shell simulation
 * @return
 */
int runMDC() {

    const int BUFF_SIZE = 128;

    // Get user input
    printf("===== Mid-Day Commander, v0 =====\n");
    printf("G’day, Commander! What command would you like to run?\n"
           "   0. whoami  : Prints out the result of the whoamicommand\n"
           "   1. last    : Prints out the result of the last command\n"
           "   2. ls      : Prints out the result of a listing on a user-specified path\n");
    printf("Option?: ");
    char buff[BUFF_SIZE];
    fgets(buff, sizeof buff, stdin);
    printf("\n");

    // Verify user input
    int input = -1;
    char *end = NULL;
    errno = 0;

    long temp = strtol(buff, &end, 10);

    if (end != buff && errno != ERANGE && temp >= INT_MIN && temp <= INT_MAX) {
        input = (int) temp;
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
//        wait(NULL);
        waitpid(cpid, NULL, 0);

        // stop timer
        gettimeofday(&t2, NULL);

        // compute and print the elapsed time in millisec
        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // tack on the us

        printChildStatistics(elapsedTime);
    } else if (cpid == 0) {
        // child process

        if (input == 0) { // whoami
            printf("-- Who Am I? --\n");

            char *const args[] = {"./ls", '\0'};
            execv("/usr/bin/whoami", args);

        } else if (input == 1) { //last
            printf("-- Last Logins --\n");

            char *const args[] = {"./last", '\0'};
            execv("/usr/bin/last", args);

        } else if (input == 2) { // ls
            printf("-- Directory Listing --\n");

            char argBuff[BUFF_SIZE];
            char pathBuff[BUFF_SIZE];

            printf("Arguments?: ");
            fgets(argBuff, sizeof argBuff, stdin);
            nullTerminateStr(argBuff);
            splitByDelim(argBuff, " ");

            printf("Path?: ");
            fgets(pathBuff, sizeof pathBuff, stdin);
            nullTerminateStr(pathBuff);
            printf("\n");

            if (strlen(argBuff) == 0 && strlen(pathBuff) == 0) { // user specified no args or path
                char *const args[] = {"./ls", '\0'};
                execv("/bin/ls", args);
            } else if (strlen(argBuff) == 0) { // user specified only a path
                char *const args[] = {"./ls", pathBuff, '\0'};
                execv("/bin/ls", args);
            } else if (strlen(pathBuff) == 0) { // user specified only arg(s)
                char *const args[] = {"./ls", argBuff, '\0'};
                execv("/bin/ls", args);
            } else { // user specified both arg(s) and a path
                char *const args[] = {"./ls", argBuff, pathBuff, '\0'};
                execv("/bin/ls", args);
            }

        } else {
            printf("Incorrect input\n");
            return (1);
        }


        exit(EXIT_FAILURE); // execv returned, meaning error
    } else {
        printf("Error, failed to fork\n");
        return 1;
    }

    return 0;
}
