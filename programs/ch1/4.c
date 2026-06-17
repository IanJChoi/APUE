#include "ourhdr.h"

int
main(void)
{
    /*
     * getpid() returns the process ID of the current process.
     * Each running process has its own unique process ID.
     */
    printf("hello world from process ID %d\n", getpid());

    /*
     * Terminate the program successfully.
     */
    exit(0);
}