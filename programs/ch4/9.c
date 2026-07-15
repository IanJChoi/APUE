#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ourhdr.h"

int
main(void)
{
    char *ptr;
    size_t size;

    /*
     * Change to the specified directory.
     *
     * In the book's example, /usr/spool is a symbolic link
     * that eventually leads to /var/spool.
     */
    if (chdir("/usr/spool/uucppublic") < 0)
        err_sys("chdir failed");

    /*
     * Allocate a buffer large enough to store a pathname.
     *
     * path_alloc() stores the allocated buffer size in size.
     */
    ptr = path_alloc(&size);

    /*
     * Obtain the absolute pathname of the current working directory.
     *
     * getcwd() returns:
     *     ptr   on success
     *     NULL  on failure
     */
    if (getcwd(ptr, size) == NULL)
        err_sys("getcwd failed");

    printf("cwd = %s\n", ptr);

    free(ptr);

    exit(0);
}