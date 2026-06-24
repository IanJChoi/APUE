#include "ourhdr.h"

int
main(void)
{
    // getuid() returns the user ID of the current process.
    // getgid() returns the group ID of the current process.
    printf("uid = %d, gid = %d\n", getuid(), getgid());

    // Terminate the program successfully.
    exit(0);
}