#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utime.h>
#include "ourhdr.h"

int
main(int argc, char *argv[])
{
    int             i;
    struct stat     statbuf;
    struct utimbuf  timebuf;

    /*
     * Process every filename given as a command-line argument.
     * argv[0] is the program name, so processing begins at argv[1].
     */
    for (i = 1; i < argc; i++) {

        /*
         * Save the file's current metadata, including:
         *
         * st_atime : last access time
         * st_mtime : last modification time
         */
        if (stat(argv[i], &statbuf) < 0) {
            err_ret("%s: stat error", argv[i]);
            continue;
        }

        /*
         * Open the file for writing and truncate it to zero bytes.
         *
         * O_RDWR  : open for reading and writing
         * O_TRUNC : set the file length to zero
         *
         * Truncating the file normally changes its modification time.
         */
        if (open(argv[i], O_RDWR | O_TRUNC) < 0) {
            err_ret("%s: open error", argv[i]);
            continue;
        }

        /*
         * Copy the original access and modification times saved by stat().
         */
        timebuf.actime  = statbuf.st_atime;
        timebuf.modtime = statbuf.st_mtime;

        /*
         * Restore the file's original timestamps.
         *
         * The file remains empty, but its access and modification times
         * are changed back to the values that existed before truncation.
         */
        if (utime(argv[i], &timebuf) < 0) {
            err_ret("%s: utime error", argv[i]);
            continue;
        }
    }

    exit(0);
}