#include <sys/types.h>   /* for basic system data types */
#include <sys/stat.h>    /* for lstat(), struct stat, and S_ISxxx macros */
#include "ourhdr.h"      /* APUE helper functions such as err_ret() */

/*
 * This program prints the file type of each command-line argument.
 *
 * It uses lstat() instead of stat() so that symbolic links are detected
 * as symbolic links. If stat() were used, the symbolic link would be
 * followed and we would see the type of the file it points to.
 */
int
main(int argc, char *argv[])
{
    int i;
    struct stat buf;
    char *ptr;

    /*
     * Start from argv[1], because argv[0] is the program name.
     * Each remaining argument is treated as a pathname.
     */
    for (i = 1; i < argc; i++) {
        printf("%s: ", argv[i]);

        /*
         * lstat() fills the struct stat object with information
         * about the file itself.
         *
         * If the file is a symbolic link, lstat() returns information
         * about the symbolic link, not the target file.
         */
        if (lstat(argv[i], &buf) < 0) {
            err_ret("lstat error");
            continue;
        }

        /*
         * The st_mode field contains both the file type and
         * permission bits.
         *
         * The S_ISxxx macros check the file type portion of st_mode.
         */
        if (S_ISREG(buf.st_mode))
            ptr = "regular";
        else if (S_ISDIR(buf.st_mode))
            ptr = "directory";
        else if (S_ISCHR(buf.st_mode))
            ptr = "character special";
        else if (S_ISBLK(buf.st_mode))
            ptr = "block special";
        else if (S_ISFIFO(buf.st_mode))
            ptr = "fifo";

#ifdef S_ISLNK
        /*
         * S_ISLNK checks whether the file is a symbolic link.
         * This is only possible because we used lstat().
         */
        else if (S_ISLNK(buf.st_mode))
            ptr = "symbolic link";
#endif

#ifdef S_ISSOCK
        /*
         * S_ISSOCK checks whether the file is a socket.
         */
        else if (S_ISSOCK(buf.st_mode))
            ptr = "socket";
#endif

        /*
         * If none of the known file type checks matched,
         * the file type is unknown to this program.
         */
        else
            ptr = "** unknown mode **";

        printf("%s\n", ptr);
    }

    exit(0);
}