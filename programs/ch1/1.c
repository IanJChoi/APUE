#include <sys/types.h>  // Basic system data types
#include <dirent.h>     // DIR, struct dirent, opendir(), readdir(), closedir()
#include "ourhdr.h"     // APUE error-handling helpers

int main(int argc, char *argv[])
{
    DIR             *dp;     // Directory stream returned by opendir()
    struct dirent   *dirp;   // One entry inside the directory

    // This program needs exactly one argument: the directory path.
    if (argc != 2)
        err_quit("a single argument (the directory name) is required");

    // Open the directory given by the user.
    // argv[0] is the program name, argv[1] is the first real argument.
    if ((dp = opendir(argv[1])) == NULL)
        err_sys("can't open %s", argv[1]);

    // Keep reading entries until readdir() returns NULL.
    // dirp->d_name is the name of the current directory entry.
    while ((dirp = readdir(dp)) != NULL)
        printf("%s\n", dirp->d_name);

    // Release the directory stream.
    closedir(dp);

    exit(0);
}