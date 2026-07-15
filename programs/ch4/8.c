#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ourhdr.h"

int
main(void)
{
    /*
     * Change the current process's working directory to /tmp.
     *
     * chdir() returns:
     *     0  on success
     *    -1  on failure
     */
    if (chdir("/tmp") < 0)
        err_sys("chdir failed");

    printf("chdir to /tmp succeeded\n");

    exit(0);
}