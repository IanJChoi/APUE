#include "ourhdr.h"
#include <errno.h>
#include <limits.h>

/*
 * If PATH_MAX is not defined, we use this guessed value.
 * POSIX allows PATH_MAX to be indeterminate, so this is only a fallback.
 */
#ifdef PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif

#define PATH_MAX_GUESS 1024   /* If PATH_MAX is indeterminate */

/*
 * Allocate memory dynamically for a pathname.
 *
 * The function returns a pointer to a malloc-allocated buffer.
 * If size is not NULL, the allocated size is stored in *size.
 */
char *
path_alloc(int *size)
{
    char *ptr;

    /*
     * If pathmax is 0, PATH_MAX was not defined at compile time.
     * Therefore, we ask the system at run time using pathconf().
     */
    if (pathmax == 0) {
        errno = 0;

        /*
         * Ask for the maximum relative pathname length
         * when the current directory is "/".
         */
        if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
            /*
             * If errno is still 0, pathconf() did not fail.
             * The value is simply indeterminate.
             */
            if (errno == 0)
                pathmax = PATH_MAX_GUESS;
            else
                err_sys("pathconf error for _PC_PATH_MAX");
        } else {
            /*
             * Add one byte for the null character '\0'
             * at the end of the C string.
             */
            pathmax++;
        }
    }

    /*
     * Allocate memory for the pathname.
     * Another +1 is used here in the APUE version to be conservative.
     */
    if ((ptr = malloc(pathmax + 1)) == NULL)
        err_sys("malloc error for pathname");

    /*
     * If the caller wants to know the allocated size,
     * store the size through the pointer argument.
     */
    if (size != NULL)
        *size = pathmax + 1;

    return ptr;
}