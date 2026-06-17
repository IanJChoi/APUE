#include "ourhdr.h"

int
main(void)
{
    int c;  // Store each character read from standard input.
            // int is used instead of char so that EOF can be represented.

    // Read one character at a time from standard input until EOF is reached.
    while ((c = getc(stdin)) != EOF)

        // Write the character to standard output.
        // If putc fails, report an output error.
        if (putc(c, stdout) == EOF)
            err_sys("output error");

    // Check whether the loop ended because of an input error.
    // EOF itself is not an error, so we must check separately.
    if (ferror(stdin))
        err_sys("input error");

    // Exit successfully.
    exit(0);
}