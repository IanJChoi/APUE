#include "ourhdr.h"
#include <errno.h>
#include <limits.h>

/*
 * If OPEN_MAX is defined, use it as the initial value.
 * Otherwise, set openmax to 0 so we can ask the system at run time.
 */
#ifdef OPEN_MAX
static int openmax = OPEN_MAX;
#else
static int openmax = 0;
#endif

#define OPEN_MAX_GUESS 256    /* If OPEN_MAX is indeterminate */

/*
 * Return the maximum number of file descriptors
 * that this process can have open at the same time.
 */
int
open_max(void)
{
    /*
     * If openmax is 0, OPEN_MAX was not defined at compile time.
     * Therefore, we ask the system at run time using sysconf().
     */
    if (openmax == 0) {
        errno = 0;

        /*
         * Ask the system for the maximum number of open files
         * per process.
         */
        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
            /*
             * If errno is still 0, sysconf() did not fail.
             * The value is simply indeterminate.
             */
            if (errno == 0)
                openmax = OPEN_MAX_GUESS;
            else
                err_sys("sysconf error for _SC_OPEN_MAX");
        }
    }

    return openmax;
}