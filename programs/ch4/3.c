#include <sys/types.h>   /* for basic system data types */
#include <sys/stat.h>    /* for umask() and permission constants */
#include <fcntl.h>       /* for creat() */
#include "ourhdr.h"      /* APUE helper functions such as err_sys() */

/*
 * This program demonstrates how umask affects the permissions
 * of newly created files.
 *
 * The requested file mode is not always the final file mode.
 * The process's umask can turn off permission bits.
 *
 * Final mode:
 *
 *     requested_mode & ~umask
 */
int
main(void)
{
    /*
     * Set the file mode creation mask to 0.
     *
     * This means no permission bits will be turned off
     * when a new file is created.
     */
    umask(0);

    /*
     * Create a file named "foo".
     *
     * Requested permissions:
     *
     *   owner: read, write
     *   group: read, write
     *   others: read, write
     *
     * Since umask is 0, the final permissions should be:
     *
     *   -rw-rw-rw-
     */
    if (creat("foo",
              S_IRUSR | S_IWUSR |
              S_IRGRP | S_IWGRP |
              S_IROTH | S_IWOTH) < 0)
        err_sys("creat error for foo");

    /*
     * Set a new umask.
     *
     * This mask turns off:
     *
     *   group read
     *   group write
     *   others read
     *   others write
     *
     * In other words, only the owner permissions will remain.
     */
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    /*
     * Create a file named "bar" with the same requested permissions
     * as "foo".
     *
     * Requested permissions:
     *
     *   -rw-rw-rw-
     *
     * But the current umask removes group and others permissions.
     *
     * Final permissions should be:
     *
     *   -rw-------
     */
    if (creat("bar",
              S_IRUSR | S_IWUSR |
              S_IRGRP | S_IWGRP |
              S_IROTH | S_IWOTH) < 0)
        err_sys("creat error for bar");

    exit(0);
}