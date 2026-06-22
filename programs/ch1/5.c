#include <sys/types.h>
#include <sys/wait.h>
#include "ourhdr.h"

int
main(void)
{
    char    buf[MAXLINE];   /* buffer to store one input line */
    pid_t   pid;            /* process ID returned by fork() */
    int     status;         /* termination status of the child process */

    /*
     * Print the prompt.
     * printf requires "%%" to print a literal percent sign.
     */
    printf("%% ");

    /*
     * Read one line at a time from standard input.
     * fgets() returns NULL when end-of-file or an error occurs.
     */
    while (fgets(buf, MAXLINE, stdin) != NULL) {

        /*
         * Remove the newline character at the end of the input line.
         * For example, "date\n" becomes "date".
         */
        buf[strlen(buf) - 1] = 0;

        /*
         * Create a new child process.
         * After fork(), both the parent and child continue execution.
         */
        if ((pid = fork()) < 0) {
            /* fork() failed */
            err_sys("fork error");
        }

        else if (pid == 0) {
            /*
             * This block is executed by the child process.
             *
             * execlp() replaces the child process with the program
             * named by buf.
             *
             * For example, if buf is "date", this runs the date command.
             */
            execlp(buf, buf, (char *) 0);

            /*
             * If execlp() succeeds, the code below is never executed.
             * Therefore, reaching here means execlp() failed.
             */
            err_ret("couldn't execute: %s", buf);
            exit(127);
        }

        /*
         * This block is executed by the parent process.
         *
         * The parent waits for the child process to finish before
         * printing another prompt.
         */
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");

        /* Print the next prompt after the child command finishes. */
        printf("%% ");
    }

    /* Exit normally when input ends. */
    exit(0);
}