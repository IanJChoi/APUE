#include <fcntl.h>
#include "ourhdr.h"

/*
 * Turn on one or more file status flags for a file descriptor.
 *
 * fd:    file descriptor whose flags we want to modify
 * flags: file status flags to turn on, such as O_APPEND or O_NONBLOCK
 */
void
set_fl(int fd, int flags)
{
    int     val;

    /*
     * First, get the current file status flags.
     * We must preserve the existing flags.
     */
    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        err_sys("fcntl F_GETFL error");

    /*
     * Turn on the requested flags while keeping the old flags.
     * The |= operator adds the new flags without clearing existing ones.
     */
    val |= flags;

    /*
     * Set the modified flag value back to the file descriptor.
     */
    if (fcntl(fd, F_SETFL, val) < 0)
        err_sys("fcntl F_SETFL error");
}