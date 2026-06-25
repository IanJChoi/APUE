#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "ourhdr.h"

/*
 * Signal-catching function for SIGINT.
 * This function is called when the user types the interrupt key,
 * usually Control-C.
 */
static void sig_int(int);

int
main(void)
{
    char    buf[MAXLINE];
    pid_t   pid;
    int     status;

    /*
     * Install a signal handler for SIGINT.
     *
     * Normally, when the user types Control-C, the process receives
     * SIGINT and terminates by default.
     *
     * Here, we tell the system to call sig_int() instead.
     */
    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal error");

    /*
     * Print the shell prompt.
     * We use "%%" because printf() uses '%' for format specifiers.
     */
    printf("%% ");

    /*
     * Read one line at a time from standard input.
     * fgets() returns NULL when end-of-file is reached.
     */
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        /*
         * Replace the newline character at the end of the input
         * with a null character so the string can be used as a command.
         */
        buf[strlen(buf) - 1] = 0;

        /*
         * Create a child process.
         * The child will execute the command entered by the user.
         */
        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid == 0) {
            /*
             * Child process.
             *
             * execlp() replaces the child process image with
             * the program specified by buf.
             *
             * The first buf is the file to execute.
             * The second buf is argv[0], the command name.
             * The final (char *) 0 marks the end of the argument list.
             */
            execlp(buf, buf, (char *) 0);

            /*
             * If execlp() returns, it means an error occurred.
             */
            err_ret("couldn't execute: %s", buf);
            exit(127);
        }

        /*
         * Parent process.
         *
         * Wait for the child process to terminate before printing
         * the next prompt.
         */
        if ((pid = waitpid(pid, &status, 0)) < 0)
            err_sys("waitpid error");

        /*
         * Print the prompt again after the command finishes.
         */
        printf("%% ");
    }

    /*
     * Exit successfully when end-of-file is reached.
     */
    exit(0);
}

/*
 * This function handles SIGINT.
 *
 * Instead of terminating the shell when Control-C is typed,
 * it prints a message and displays a new prompt.
 */
static void
sig_int(int signo)
{
    printf("interrupt\n%% ");
}