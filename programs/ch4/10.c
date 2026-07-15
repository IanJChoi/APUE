#include <sys/types.h>
#include <sys/stat.h>

#ifdef __linux__
#include <sys/sysmacros.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ourhdr.h"

int
main(int argc, char *argv[])
{
    int i;
    struct stat buf;

    /*
     * Process each pathname provided on the command line.
     */
    for (i = 1; i < argc; i++) {
        printf("%s: ", argv[i]);

        /*
         * Use lstat() so that information about a symbolic link
         * itself is returned instead of information about its target.
         */
        if (lstat(argv[i], &buf) < 0) {
            err_ret("lstat error");
            continue;
        }

        /*
         * st_dev identifies the device containing the filesystem
         * where this pathname and its inode are stored.
         *
         * major() extracts the major device number.
         * minor() extracts the minor device number.
         */
        printf(
            "dev = %d/%d",
            (int) major(buf.st_dev),
            (int) minor(buf.st_dev)
        );

        /*
         * st_rdev is meaningful only for character special files
         * and block special files.
         *
         * It identifies the actual device represented by the
         * special file.
         */
        if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode)) {
            printf(
                " (%s) rdev = %d/%d",
                S_ISCHR(buf.st_mode) ? "character" : "block",
                (int) major(buf.st_rdev),
                (int) minor(buf.st_rdev)
            );
        }

        printf("\n");
    }

    exit(0);
}