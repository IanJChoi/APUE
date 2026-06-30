#include <errno.h>
#include "ourhdr.h"

/*
 * Print a system-wide run-time limit using sysconf().
 *
 * Example:
 *      sysconf(_SC_OPEN_MAX)
 *
 * This is used for limits that are not associated with
 * a particular file or directory.
 */
static void pr_sysconf(char *, int);

/*
 * Print a path-dependent run-time limit using pathconf().
 *
 * Example:
 *      pathconf("/tmp", _PC_NAME_MAX)
 *
 * This is used for limits that can depend on a particular
 * pathname, directory, filesystem, pipe, FIFO, or terminal.
 */
static void pr_pathconf(char *, char *, int);

int
main(int argc, char *argv[])
{
    /*
     * This program expects one command-line argument:
     * a directory name.
     *
     * Some pathconf() values, such as NAME_MAX and PATH_MAX,
     * must be checked relative to a specific directory.
     */
    if (argc != 2)
        err_quit("usage: a.out <dirname>");

    /*
     * These values are system-wide limits.
     * Therefore, they are obtained with sysconf().
     */

    pr_sysconf("ARG_MAX                 =", _SC_ARG_MAX);
    pr_sysconf("CHILD_MAX               =", _SC_CHILD_MAX);
    pr_sysconf("clock ticks/second      =", _SC_CLK_TCK);
    pr_sysconf("NGROUPS_MAX             =", _SC_NGROUPS_MAX);
    pr_sysconf("OPEN_MAX                =", _SC_OPEN_MAX);

    /*
     * Some constants may not be defined on every system.
     * We use #ifdef so the program can still compile on systems
     * that do not define these constants.
     */
#ifdef _SC_STREAM_MAX
    pr_sysconf("STREAM_MAX              =", _SC_STREAM_MAX);
#endif

#ifdef _SC_TZNAME_MAX
    pr_sysconf("TZNAME_MAX              =", _SC_TZNAME_MAX);
#endif

    /*
     * These are POSIX feature-test values.
     * They tell us whether certain optional POSIX features
     * are supported by this implementation.
     */
    pr_sysconf("_POSIX_JOB_CONTROL      =", _SC_JOB_CONTROL);
    pr_sysconf("_POSIX_SAVED_IDS        =", _SC_SAVED_IDS);
    pr_sysconf("_POSIX_VERSION          =", _SC_VERSION);

#ifdef _SC_XOPEN_VERSION
    pr_sysconf("_XOPEN_VERSION          =", _SC_XOPEN_VERSION);
#endif

    /*
     * These values are terminal-related.
     * Therefore, we query them using /dev/tty.
     */
    pr_pathconf("MAX_CANON               =", "/dev/tty", _PC_MAX_CANON);
    pr_pathconf("MAX_INPUT               =", "/dev/tty", _PC_MAX_INPUT);
    pr_pathconf("_POSIX_VDISABLE         =", "/dev/tty", _PC_VDISABLE);

    /*
     * These values are path-dependent.
     * We use the directory provided by the user as argv[1].
     */
    pr_pathconf("LINK_MAX                =", argv[1], _PC_LINK_MAX);
    pr_pathconf("NAME_MAX                =", argv[1], _PC_NAME_MAX);
    pr_pathconf("PATH_MAX                =", argv[1], _PC_PATH_MAX);
    pr_pathconf("PIPE_BUF                =", argv[1], _PC_PIPE_BUF);
    pr_pathconf("_POSIX_NO_TRUNC         =", argv[1], _PC_NO_TRUNC);
    pr_pathconf("_POSIX_CHOWN_RESTRICTED =", argv[1], _PC_CHOWN_RESTRICTED);

    exit(0);
}

/*
 * Print the value returned by sysconf().
 *
 * Important point:
 * sysconf() can return -1 in two different cases:
 *
 * 1. Real error:
 *      return value == -1 and errno is changed
 *
 * 2. Limit is indeterminate or not defined:
 *      return value == -1 and errno is still 0
 *
 * Therefore, errno must be set to 0 before calling sysconf().
 */
static void
pr_sysconf(char *mesg, int name)
{
    long val;

    fputs(mesg, stdout);

    /*
     * Set errno to 0 before the call so we can distinguish
     * a real error from an indeterminate value.
     */
    errno = 0;

    if ((val = sysconf(name)) < 0) {
        if (errno != 0)
            err_sys("sysconf error");

        /*
         * If errno is still 0, this is not a real error.
         * It means the value is not defined or indeterminate.
         */
        fputs(" (not defined)\n", stdout);
    } else {
        printf(" %ld\n", val);
    }
}

/*
 * Print the value returned by pathconf().
 *
 * pathconf() is used for limits that depend on a specific path.
 *
 * Like sysconf(), pathconf() can return -1 in two different cases:
 *
 * 1. Real error:
 *      return value == -1 and errno is changed
 *
 * 2. No fixed limit / indeterminate:
 *      return value == -1 and errno is still 0
 */
static void
pr_pathconf(char *mesg, char *path, int name)
{
    long val;

    fputs(mesg, stdout);

    /*
     * Reset errno before calling pathconf().
     * This lets us check whether -1 means an error or no fixed limit.
     */
    errno = 0;

    if ((val = pathconf(path, name)) < 0) {
        if (errno != 0)
            err_sys("pathconf error, path = %s", path);

        /*
         * If errno is still 0, the value is not a real error.
         * It means there is no fixed limit or the value is indeterminate.
         */
        fputs(" (no limit)\n", stdout);
    } else {
        printf(" %ld\n", val);
    }
}