#include <sys/types.h>   /* for basic system data types */
#include <fcntl.h>       /* for open() and O_RDONLY */
#include "ourhdr.h"      /* APUE helper functions such as err_quit() and err_ret() */

/*
 * This program demonstrates the difference between access() and open().
 *
 * access() checks permissions using the real user ID and real group ID.
 * open() checks permissions using the effective user ID and effective group ID.
 *
 * This difference is especially important for set-user-ID programs.
 */
int
main(int argc, char *argv[])
{
    /*
     * The program expects exactly one pathname argument.
     */
    if (argc != 2)
        err_quit("usage: a.out <pathname>");

    /*
     * access() checks whether the real user can read the file.
     *
     * R_OK means "test for read permission."
     *
     * If this fails, the real user does not have read permission
     * for the file.
     */
    if (access(argv[1], R_OK) < 0)
        err_ret("access error for %s", argv[1]);
    else
        printf("read access OK\n");

    /*
     * open() actually tries to open the file for reading.
     *
     * Unlike access(), open() uses the effective user ID and
     * effective group ID for permission checking.
     *
     * Therefore, in a set-user-ID program, open() can succeed
     * even when access() fails.
     */
    if (open(argv[1], O_RDONLY) < 0)
        err_ret("open error for %s", argv[1]);
    else
        printf("open for reading OK\n");

    exit(0);
}