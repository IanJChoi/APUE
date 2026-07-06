#include <sys/types.h>
#include "ourhdr.h"

int
main(void)
{
    /*
     * Try to get the current file offset of standard input.
     *
     * lseek(fd, 0, SEEK_CUR) does not move the offset.
     * It simply asks the kernel for the current offset.
     *
     * If standard input refers to a regular file, this succeeds.
     * If standard input refers to a pipe or FIFO, this fails because
     * pipes and FIFOs do not support seeking.
     */
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
        printf("cannot seek\n");
    else
        printf("seek OK\n");

    /*
     * Exit successfully.
     */
    exit(0);
}