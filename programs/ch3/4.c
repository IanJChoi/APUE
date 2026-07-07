#include <sys/types.h>
#include <fcntl.h>
#include "ourhdr.h"

int
main(int argc, char *argv[])
{
    int     accmode, val;

    /*
     * The program expects exactly one command-line argument:
     * the file descriptor number to inspect.
     */
    if (argc != 2)
        err_quit("usage: a.out <descriptor#>");

    /*
     * Get the file status flags for the given file descriptor.
     * F_GETFL returns flags such as O_RDONLY, O_WRONLY, O_APPEND, etc.
     */
    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
        err_sys("fcntl error for fd %d", atoi(argv[1]));

    /*
     * Extract only the access mode bits.
     * O_RDONLY, O_WRONLY, and O_RDWR are not independent bit flags,
     * so we must mask with O_ACCMODE first.
     */
    accmode = val & O_ACCMODE;

    /*
     * Print the access mode.
     */
    if (accmode == O_RDONLY)
        printf("read only");
    else if (accmode == O_WRONLY)
        printf("write only");
    else if (accmode == O_RDWR)
        printf("read write");
    else
        err_dump("unknown access mode");

    /*
     * Print additional file status flags if they are enabled.
     */
    if (val & O_APPEND)
        printf(", append");

    if (val & O_NONBLOCK)
        printf(", nonblocking");

#if !defined(_POSIX_SOURCE) && defined(O_SYNC)
    if (val & O_SYNC)
        printf(", synchronous writes");
#endif

    putchar('\n');
    exit(0);
}