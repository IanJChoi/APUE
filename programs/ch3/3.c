#include "ourhdr.h"

#define BUFFSIZE 8192

int
main(void)
{
    int     n;
    char    buf[BUFFSIZE];

    /*
     * Read up to BUFFSIZE bytes from standard input.
     * read() returns the number of bytes actually read.
     * The loop continues while read() returns a positive value.
     */
    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
        /*
         * Write exactly the number of bytes that were read.
         * If write() writes fewer bytes than requested, treat it as an error.
         */
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");
    }

    /*
     * If read() returns a negative value, an error occurred.
     * If read() returns 0, it means end-of-file.
     */
    if (n < 0)
        err_sys("read error");

    exit(0);
}