#include <sys/types.h>
#include <sys/stat.h>
#include "ourhdr.h"

int
main(void)
{
    struct stat statbuf;

    /*
     * Get the current file information for "foo".
     * We need its existing permission bits before modifying them.
     */
    if (stat("foo", &statbuf) < 0)
        err_sys("stat error for foo");

    /*
     * Change the permissions of "foo":
     *
     * statbuf.st_mode & ~S_IXGRP
     *     Turn off the group-execute permission bit.
     *
     * | S_ISGID
     *     Turn on the set-group-ID bit.
     *
     * All other existing permission bits remain unchanged.
     */
    if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
        err_sys("chmod error for foo");

    /*
     * Set the permissions of "bar" to an absolute value:
     *
     * S_IRUSR : owner can read
     * S_IWUSR : owner can write
     * S_IRGRP : group can read
     * S_IROTH : others can read
     *
     * Result: rw-r--r--
     */
    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
        err_sys("chmod error for bar");

    exit(0);
}ㄴ