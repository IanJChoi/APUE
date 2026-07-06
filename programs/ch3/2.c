#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ourhdr.h"

/*
 * These two buffers each contain 10 bytes of actual data.
 */
char    buf1[] = "abcdefghij";
char    buf2[] = "ABCDEFGHIJ";

int
main(void)
{
    int     fd;

    /*
     * Create a new file named "file.hole".
     *
     * creat() opens the file for writing.
     * FILE_MODE is usually defined in ourhdr.h, often as:
     *
     *      #define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
     *
     * If creat() fails, print an error message and terminate.
     */
    if ((fd = creat("file.hole", FILE_MODE)) < 0)
        err_sys("creat error");

    /*
     * Write 10 bytes to the beginning of the file.
     *
     * After this write, the file offset becomes 10.
     *
     * File contents so far:
     * bytes 0-9:   abcdefghij
     */
    if (write(fd, buf1, 10) != 10)
        err_sys("buf1 write error");

    /*
     * offset now = 10
     */

    /*
     * Move the file offset to byte position 40.
     *
     * This does not write any data.
     * It only changes the current file offset in the kernel.
     *
     * Bytes 10-39 are skipped. This skipped region becomes a file hole
     * after we write data at offset 40.
     */
    if (lseek(fd, 40, SEEK_SET) == -1)
        err_sys("lseek error");

    /*
     * offset now = 40
     */

    /*
     * Write another 10 bytes starting at byte position 40.
     *
     * File contents after this write:
     * bytes 0-9:    abcdefghij
     * bytes 10-39:  hole
     * bytes 40-49:  ABCDEFGHIJ
     *
     * The total file size becomes 50 bytes.
     */
    if (write(fd, buf2, 10) != 10)
        err_sys("buf2 write error");

    /*
     * offset now = 50
     */

    /*
     * Exit successfully.
     */
    exit(0);
}