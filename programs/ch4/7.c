#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ourhdr.h"

/*
 * Function type called for every file encountered.
 *
 * pathname:
 *     Full pathname of the file.
 *
 * statptr:
 *     File information returned by lstat().
 *
 * type:
 *     Indicates whether the pathname is a file, directory,
 *     unreadable directory, or a pathname that could not be examined.
 */
typedef int Myfunc(
    const char *pathname,
    const struct stat *statptr,
    int type
);

static Myfunc myfunc;
static int myftw(char *pathname, Myfunc *func);
static int dopath(Myfunc *func);

/* Counters for the different file types. */
static long nreg;    /* Regular files */
static long ndir;    /* Directories */
static long nblk;    /* Block special files */
static long nchr;    /* Character special files */
static long nfifo;   /* FIFOs */
static long nslink;  /* Symbolic links */
static long nsock;   /* Sockets */
static long ntot;    /* Total number of files */

int
main(int argc, char *argv[])
{
    int ret;

    /* Exactly one starting pathname must be supplied. */
    if (argc != 2)
        err_quit("usage: ftw <starting-pathname>");

    /*
     * Recursively traverse the directory hierarchy.
     * myfunc() will be called for every pathname encountered.
     */
    ret = myftw(argv[1], myfunc);

    /* Calculate the total number of entries found. */
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;

    /*
     * Avoid division by zero when the traversal does not count
     * any entries.
     */
    if (ntot == 0)
        ntot = 1;

    /* Print the count and percentage of each file type. */
    printf("regular files  = %7ld, %5.2f %%\n",
           nreg, nreg * 100.0 / ntot);

    printf("directories    = %7ld, %5.2f %%\n",
           ndir, ndir * 100.0 / ntot);

    printf("block special  = %7ld, %5.2f %%\n",
           nblk, nblk * 100.0 / ntot);

    printf("char special   = %7ld, %5.2f %%\n",
           nchr, nchr * 100.0 / ntot);

    printf("FIFOs          = %7ld, %5.2f %%\n",
           nfifo, nfifo * 100.0 / ntot);

    printf("symbolic links = %7ld, %5.2f %%\n",
           nslink, nslink * 100.0 / ntot);

    printf("sockets        = %7ld, %5.2f %%\n",
           nsock, nsock * 100.0 / ntot);

    exit(ret);
}

/*
 * Values passed to the callback function.
 */
#define FTW_F    1   /* File other than a directory */
#define FTW_D    2   /* Directory */
#define FTW_DNR  3   /* Directory that cannot be read */
#define FTW_NS   4   /* File whose status cannot be obtained */

static char *fullpath;

/*
 * Begin traversing the directory hierarchy at pathname.
 */
static int
myftw(char *pathname, Myfunc *func)
{
    /*
     * Allocate enough memory to store a maximum-length pathname.
     * path_alloc() is the helper function from APUE Program 2.2.
     */
    fullpath = path_alloc(NULL);

    /* Initialize the shared pathname buffer. */
    strcpy(fullpath, pathname);

    /* Begin recursive traversal. */
    return dopath(func);
}

/*
 * Recursively process the pathname currently stored in fullpath.
 *
 * If fullpath is not a directory, call the callback function
 * and return.
 *
 * If fullpath is a directory, call the callback for the directory
 * and then recursively process every entry inside it.
 */
static int
dopath(Myfunc *func)
{
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret;
    char *ptr;

    /*
     * Use lstat() instead of stat() so symbolic links themselves
     * are examined instead of following the links.
     */
    if (lstat(fullpath, &statbuf) < 0)
        return func(fullpath, &statbuf, FTW_NS);

    /*
     * If this pathname is not a directory, process it as a file.
     */
    if (S_ISDIR(statbuf.st_mode) == 0)
        return func(fullpath, &statbuf, FTW_F);

    /*
     * The pathname is a directory.
     * First process the directory itself.
     */
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
        return ret;

    /*
     * Move ptr to the end of the pathname and append a slash.
     *
     * For example:
     *     /home/user
     * becomes:
     *     /home/user/
     */
    ptr = fullpath + strlen(fullpath);
    *ptr++ = '/';
    *ptr = '\0';

    /*
     * Open the directory.
     */
    if ((dp = opendir(fullpath)) == NULL)
        return func(fullpath, &statbuf, FTW_DNR);

    /*
     * Read every entry in the directory.
     */
    while ((dirp = readdir(dp)) != NULL) {
        /*
         * Ignore "." and ".." to prevent infinite recursion.
         */
        if (strcmp(dirp->d_name, ".") == 0 ||
            strcmp(dirp->d_name, "..") == 0)
            continue;

        /* Append the directory entry name after the slash. */
        strcpy(ptr, dirp->d_name);

        /* Recursively process the new pathname. */
        if ((ret = dopath(func)) != 0)
            break;
    }

    /*
     * Remove the slash and filename that were appended,
     * restoring fullpath to the parent directory pathname.
     */
    ptr[-1] = '\0';

    /* Close the directory stream. */
    if (closedir(dp) < 0)
        err_ret("can't close directory %s", fullpath);

    return ret;
}

/*
 * Callback function invoked for every pathname.
 *
 * It increases the appropriate counter based on the pathname type.
 */
static int
myfunc(
    const char *pathname,
    const struct stat *statptr,
    int type
)
{
    switch (type) {
    case FTW_F:
        /*
         * The pathname is not a directory.
         * Determine its exact file type from st_mode.
         */
        switch (statptr->st_mode & S_IFMT) {
        case S_IFREG:
            nreg++;
            break;

        case S_IFBLK:
            nblk++;
            break;

        case S_IFCHR:
            nchr++;
            break;

        case S_IFIFO:
            nfifo++;
            break;

        case S_IFLNK:
            nslink++;
            break;

#ifdef S_IFSOCK
        case S_IFSOCK:
            nsock++;
            break;
#endif

        case S_IFDIR:
            /*
             * Directories should have been passed with FTW_D,
             * not FTW_F.
             */
            err_dump("for S_IFDIR for %s", pathname);
        }

        break;

    case FTW_D:
        /* Count a readable directory. */
        ndir++;
        break;

    case FTW_DNR:
        /* The directory exists but cannot be opened. */
        err_ret("can't read directory %s", pathname);
        break;

    case FTW_NS:
        /* lstat() failed for this pathname. */
        err_ret("stat error for %s", pathname);
        break;

    default:
        err_dump(
            "unknown type %d for pathname %s",
            type,
            pathname
        );
    }

    /*
     * Returning zero tells the traversal function to continue.
     */
    return 0;
}