#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ourhdr.h"

int
main(void)
{
    /*
     * Open "tempfile" for both reading and writing.
     *
     * The returned file descriptor is intentionally not saved because
     * this example only needs the file to remain open.
     */
    if (open("tempfile", O_RDWR) < 0)
        err_sys("open error");

    /*
     * Remove the directory entry for "tempfile".
     *
     * After this call, the filename disappears from the directory.
     * However, the file's data is not deleted yet because this process
     * still has the file open.
     */
    if (unlink("tempfile") < 0)
        err_sys("unlink error");

    printf("file unlinked\n");

    /*
     * Wait for 15 seconds so that another shell can check that:
     *
     * 1. The filename no longer exists.
     * 2. The disk space has not yet been released.
     */
    sleep(15);

    printf("done\n");

    /*
     * When the process exits, the kernel closes the open file descriptor.
     * Since the file has no remaining directory links, its data and disk
     * space can now be released.
     */
    exit(0);
}