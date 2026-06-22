#include <errno.h>      // Defines errno and error constants such as EACCES and ENOENT
#include "ourhdr.h"    // APUE custom header; includes common headers and function declarations

int
main(int argc, char *argv[])
{
    /*
     * strerror() converts an error number into a human-readable
     * error message string.
     *
     * EACCES usually means "Permission denied".
     *
     * fprintf(stderr, ...) prints the message to standard error.
     */
    fprintf(stderr, "EACCES: %s\n", strerror(EACCES));

    /*
     * Set errno manually to ENOENT for demonstration.
     *
     * ENOENT usually means "No such file or directory".
     *
     * In real programs, errno is normally set by a system call
     * or library function when an error occurs.
     */
    errno = ENOENT;

    /*
     * perror() prints an error message based on the current value of errno.
     *
     * The output format is:
     *
     *     msg: error message
     *
     * argv[0] is the name used to run this program.
     * For example, if the program is executed as ./a.out,
     * argv[0] is usually "./a.out" or "a.out".
     */
    perror(argv[0]);

    /*
     * Exit successfully.
     */
    exit(0);
}