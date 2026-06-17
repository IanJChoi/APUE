#include "ourhdr.h"

#define BUFFSIZE 8192   // Size of the buffer used for each read operation.

int main(void)
{
    int     n;                  // Number of bytes actually read.
    char    buf[BUFFSIZE];      // Temporary buffer to store input data.

    /*
     * Keep reading from standard input.
     * read() returns the number of bytes read.
     * If it returns 0, it means end-of-file.
     */
    while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0)

        /*
         * Write the same number of bytes to standard output.
         * If write() writes fewer bytes than expected, report an error.
         */
        if (write(STDOUT_FILENO, buf, n) != n)
            err_sys("write error");

    /*
     * If read() returns a negative value,
     * it means an error occurred while reading.
     */
    if (n < 0)
        err_sys("read error");

    exit(0);    // Exit normally.
}