# APUE Study Notes

My notes and practice programs while studying *Advanced Programming in the UNIX Environment*.

## Chapter 1

#### Program 1.1 — List files in a directory

File: `programs/ch1/1.c`

This program is a minimal version of `ls`.

It opens a directory using `opendir()`, reads each directory entry using
`readdir()`, prints the entry name stored in `dirp->d_name`, and then
closes the directory using `closedir()`.

##### Key concepts

- `DIR *` represents an open directory stream.
- `struct dirent` represents one directory entry.
- `opendir()` opens a directory.
- `readdir()` reads entries from the directory one by one.
- `closedir()` closes the directory stream.
- `argc` and `argv` are used to read command-line arguments.

#### Program 1.2 — Copy standard input to standard output

File: `programs/ch1/2.c`

This program is a minimal version of `cat`.

It reads data from standard input using `read()`, stores the data temporarily
in a buffer, writes the same data to standard output using `write()`, and
continues until there is no more input.

##### Key concepts

- File descriptors are small integers used by the kernel to identify open files.
- `STDIN_FILENO` represents standard input, usually file descriptor `0`.
- `STDOUT_FILENO` represents standard output, usually file descriptor `1`.
- `read()` reads bytes from a file descriptor into a buffer.
- `write()` writes bytes from a buffer to a file descriptor.
- The return value of `read()` is the number of bytes actually read.
- A return value of `0` from `read()` means end-of-file.
- A negative return value from `read()` means an error occurred.
- The buffer `buf` temporarily stores data between `read()` and `write()`.
- This program uses unbuffered I/O because it works directly with file descriptors.

#### Program 1.3 — Copy standard input to standard output

File: `programs/ch1/3.c`

This program copies standard input to standard output using standard I/O.

It reads one character at a time from `stdin` using `getc()` and writes each character to `stdout` using `putc()`. The loop continues until `getc()` returns `EOF`.

##### Key concepts

- `stdin` represents standard input.
- `stdout` represents standard output.
- `getc()` reads one character from a stream.
- `putc()` writes one character to a stream.
- `EOF` indicates the end of input.
- `ferror()` checks whether an input error occurred.
- `int c` is used so the program can store both character values and `EOF`.

#### Program 1.4 — Print the process ID

File: `programs/ch1/4.c`

This program prints the process ID of the currently running process.

It calls `getpid()` to obtain the process ID of the current process, then
prints it using `printf()`. Each time this program is executed, the operating
system creates a new process, so the process ID may be different each time.

##### Key concepts

- A **program** is an executable file stored on disk.
- A **process** is an executing instance of a program.
- Every Unix process has a unique process ID.
- `getpid()` returns the process ID of the current process.
- `printf()` prints formatted output to standard output.
- `exit(0)` terminates the program successfully.

#### Program 1.5 — Read commands from standard input and execute them

File: `programs/ch1/5.c`

This program is a very small Unix shell.

It prints a prompt, reads a command from standard input, creates a new child
process using `fork()`, and executes the command in the child process using
`execlp()`. The parent process waits for the child process to finish by calling
`waitpid()`, then prints the prompt again.

For example, if the user types `date`, the program creates a child process.
The child process then executes the `date` command.

##### Key concepts

- A **shell** is a program that reads commands and executes them.
- `fgets()` reads one line from standard input.
- The newline character at the end of the input line is replaced with `'\0'`.
- `fork()` creates a new child process.
- After `fork()`, both the parent and child processes continue execution.
- In the child process, `fork()` returns `0`.
- In the parent process, `fork()` returns the process ID of the child.
- `execlp()` replaces the current process image with a new program.
- If `execlp()` succeeds, the code after it is not executed.
- If `execlp()` fails, the child prints an error message and exits.
- `waitpid()` makes the parent process wait for the child process to terminate.
- `status` stores information about how the child process terminated.
- The parent prints another prompt after the child command finishes.

#### Program 1.6 — Demonstrate `strerror()` and `perror()`

File: `programs/ch1/6.c`

This program demonstrates two standard C error-handling functions:
`strerror()` and `perror()`.

First, it uses `strerror(EACCES)` to convert the error number `EACCES`
into a readable error message.

Then, it manually sets `errno` to `ENOENT` and calls `perror(argv[0])`.
The `perror()` function prints the program name, followed by the error
message corresponding to the current value of `errno`.

##### Key concepts

- `errno` stores an error code when a system call or library function fails.
- `EACCES` usually represents a permission error.
- `ENOENT` usually represents a missing file or directory.
- `strerror()` converts an error number into a readable string.
- `perror()` prints an error message based on the current value of `errno`.
- `stderr` is used for error output.
- `argv[0]` usually contains the name of the program being executed.

#### Program 1.7 — Print user ID and group ID

File: `programs/ch1/7.c`

This program prints the user ID and group ID of the current process.

It calls `getuid()` to obtain the numeric user ID of the process, and
`getgid()` to obtain the numeric group ID of the process. These IDs are
assigned by the system and are used by Unix to identify users and groups
when checking permissions.

##### Key concepts

- A user ID, or UID, is a numeric value that identifies a user.
- A group ID, or GID, is a numeric value that identifies a group.
- `getuid()` returns the user ID of the current process.
- `getgid()` returns the group ID of the current process.
- Unix uses numeric IDs internally instead of user names and group names.
- UID 0 represents the superuser, also called `root`.
- User IDs and group IDs are important for file permission checks.

#### Program 1.8 — Read commands from standard input and execute them

File: `programs/ch1/8.c`

This program is a simple shell that reads commands from standard input
and executes them using a child process.

It is based on the earlier shell example, but adds signal handling for
`SIGINT`. Normally, when the user types Control-C, the process receives
`SIGINT` and terminates. This program changes that behavior by installing
a signal handler with `signal()`.

When `SIGINT` occurs, the function `sig_int()` is called instead of
terminating the shell. The handler prints an interrupt message and then
prints a new prompt.

##### Key concepts

- `SIGINT` is usually generated when the user types Control-C.
- The default action for `SIGINT` is to terminate the process.
- `signal()` installs a signal handler for a specific signal.
- A signal handler is a function called when a signal occurs.
- `SIG_ERR` indicates that installing the signal handler failed.
- `fork()` creates a child process.
- `execlp()` replaces the child process with a new program.
- `waitpid()` makes the parent wait for the child process to finish.
- `printf("%% ")` prints a literal `%` prompt.

## Chapter 2

#### Program 2.1 — Print system limits

File: `programs/ch2/1.c`

This program prints various system limits and path-dependent limits.

It uses `sysconf()` to obtain limits that apply to the whole system, such as
the maximum number of open files per process, the maximum argument length for
`exec` functions, and the number of clock ticks per second.

It uses `pathconf()` to obtain limits that depend on a specific pathname,
such as the maximum filename length, maximum pathname length, pipe buffer size,
and whether certain POSIX options are supported for that path.

The program also handles the special case where a limit is not defined or has
no fixed value. Since `sysconf()` and `pathconf()` can return `-1` both for
real errors and for indeterminate limits, the program sets `errno` to `0`
before each call and checks whether `errno` changed afterward.

##### Key concepts

- `sysconf()` returns system-wide run-time limits.
- `pathconf()` returns limits associated with a specific path.
- `_SC_` constants are used with `sysconf()`.
- `_PC_` constants are used with `pathconf()` and `fpathconf()`.
- Some limits are known at compile time through header files.
- Some limits must be determined at run time.
- Some limits depend on the filesystem or directory being checked.
- `errno` must be cleared before calling `sysconf()` or `pathconf()` to distinguish an error from an indeterminate value.
- A return value of `-1` with `errno != 0` means a real error occurred.
- A return value of `-1` with `errno == 0` means the limit is not defined or has no fixed value.
- `OPEN_MAX` describes the maximum number of open files per process.
- `ARG_MAX` describes the maximum size of arguments passed to `exec` functions.
- `NAME_MAX` describes the maximum length of a filename in a directory.
- `PATH_MAX` describes the maximum length of a relative pathname for a directory.
- `PIPE_BUF` describes the number of bytes that can be written atomically to a pipe or FIFO.
- Portable Unix programs should not assume fixed numeric limits; they should query the system when necessary.

#### Program 2.2 — Dynamically allocate space for a pathname

File: `programs/ch2/2.c`

This program defines a function named `path_alloc()` that dynamically
allocates memory for a pathname.

A pathname can have different maximum lengths on different systems. Some
systems define `PATH_MAX` in `<limits.h>`, but other systems may not define
it. If `PATH_MAX` is not available at compile time, this program calls
`pathconf()` with `_PC_PATH_MAX` to ask the system for the pathname limit at
run time.

If the pathname limit is indeterminate, the program uses a guessed fallback
value named `PATH_MAX_GUESS`. After deciding the pathname size, the function
calls `malloc()` to allocate memory and returns a pointer to the allocated
buffer.

The function also stores the allocated size in the variable pointed to by
the `size` argument, if `size` is not `NULL`.

##### Key concepts

- `PATH_MAX` is the maximum length of a pathname, if it is defined.
- Some systems do not define `PATH_MAX` at compile time.
- `pathconf()` is used to obtain pathname-related limits at run time.
- `_PC_PATH_MAX` is passed to `pathconf()` to ask for the pathname limit.
- A return value of `-1` from `pathconf()` does not always mean an error.
- `errno` is set to `0` before calling `pathconf()` to distinguish an error
  from an indeterminate value.
- If `errno` remains `0`, the limit is indeterminate.
- `PATH_MAX_GUESS` is used when the pathname limit cannot be determined.
- `malloc()` dynamically allocates memory for the pathname buffer.
- One extra byte is needed for the null character `'\0'` at the end of a C string.
- The returned pointer must eventually be freed by the caller using `free()`.

#### Program 2.3 — Determine the number of file descriptors

File: `programs/ch2/3.c`

This program defines a function named `open_max()` that returns the maximum
number of file descriptors that a process can have open at the same time.

Some systems define `OPEN_MAX` in `<limits.h>`, but other systems may not
define it. If `OPEN_MAX` is not available at compile time, this program calls
`sysconf()` with `_SC_OPEN_MAX` to ask the system for the limit at run time.

If the open-file limit is indeterminate, the program uses a guessed fallback
value named `OPEN_MAX_GUESS`.

This function is useful when a program needs to loop through all possible
file descriptors, such as when a daemon process closes all inherited open
files.

##### Key concepts

- A file descriptor is a small integer used by Unix to refer to an open file.
- File descriptors `0`, `1`, and `2` usually represent standard input,
  standard output, and standard error.
- `OPEN_MAX` is the maximum number of files a process can have open at once,
  if it is defined.
- Some systems do not define `OPEN_MAX` at compile time.
- `sysconf()` is used to obtain system limits at run time.
- `_SC_OPEN_MAX` is passed to `sysconf()` to ask for the open-file limit.
- A return value of `-1` from `sysconf()` does not always mean an error.
- `errno` is set to `0` before calling `sysconf()` to distinguish an error
  from an indeterminate value.
- If `errno` remains `0`, the limit is indeterminate.
- `OPEN_MAX_GUESS` is used when the open-file limit cannot be determined.
- This value can be used in a loop to close all possible file descriptors.

#### Program 3.1 — Test whether standard input is seekable

File: `programs/ch3/1.c`

This program tests whether standard input is capable of seeking.

It calls `lseek()` on `STDIN_FILENO` with an offset of `0` and `SEEK_CUR`.
This does not move the file offset. Instead, it checks the current file
offset and returns it if seeking is supported.

If standard input refers to a regular file, `lseek()` succeeds and the
program prints `seek OK`.

If standard input refers to a pipe or FIFO, `lseek()` fails because pipes
and FIFOs do not have a file offset that can be moved. In that case, the
program prints `cannot seek`.

##### Key concepts

- `lseek()` changes or checks the current file offset.
- `STDIN_FILENO` is the file descriptor for standard input.
- `SEEK_CUR` means the offset is relative to the current file position.
- `lseek(fd, 0, SEEK_CUR)` checks the current offset without changing it.
- A regular file is usually seekable.
- A pipe is not seekable because it is a sequential data stream.
- A FIFO is also not seekable.
- If `lseek()` succeeds, it returns the new file offset.
- If `lseek()` fails, it returns `-1`.
- If `lseek()` is used on a pipe or FIFO, `errno` is set to `ESPIPE`.

#### Program 3.2 — Create a file with a hole in it

File: `programs/ch3/2.c`

This program creates a file named `file.hole` and intentionally leaves an
unwritten region in the middle of the file.

First, it creates the file using `creat()`. Then it writes 10 bytes,
`abcdefghij`, at the beginning of the file. After this write, the file
offset becomes 10.

Next, the program calls `lseek(fd, 40, SEEK_SET)`. This moves the file
offset to byte position 40 without writing anything to bytes 10 through 39.

Finally, it writes another 10 bytes, `ABCDEFGHIJ`, starting at offset 40.
As a result, the file size becomes 50 bytes, but the middle 30 bytes were
never actually written. This unwritten region is called a file hole.

When the hole is read back, it appears as zero bytes.

##### Key concepts

- `creat()` creates a new file or truncates an existing file.
- `FILE_MODE` defines the permissions used when creating the file.
- `write()` writes bytes starting at the current file offset.
- After a successful `write()`, the file offset increases by the number of bytes written.
- `lseek(fd, 40, SEEK_SET)` moves the file offset to byte position 40.
- `SEEK_SET` means the offset is relative to the beginning of the file.
- `lseek()` itself does not read or write any data.
- Seeking past the end of a file is allowed.
- Writing after seeking past the end creates a hole in the file.
- A file hole is an unwritten region inside a file.
- When a file hole is read, it returns bytes with value `0`.
- Files with holes are often called sparse files.

#### Program 3.3 — Copy standard input to standard output

File: `programs/ch3/3.c`

This program copies data from standard input to standard output.

It repeatedly calls `read()` to read bytes from `STDIN_FILENO` into a buffer,
and then calls `write()` to write those bytes to `STDOUT_FILENO`. The program
does not open files directly. Instead, input and output files can be connected
by the shell using redirection.

##### Key concepts

- `read()` reads bytes from a file descriptor.
- `write()` writes bytes to a file descriptor.
- `STDIN_FILENO` represents standard input, usually file descriptor 0.
- `STDOUT_FILENO` represents standard output, usually file descriptor 1.
- Shell redirection can connect files to standard input and standard output.
- Unix treats text files and binary files as byte streams at this level.
- A larger buffer can reduce the number of system calls and improve I/O efficiency.

#### Program 3.4 — Print file flags for a file descriptor

File: `programs/ch3/4.c`

This program prints the file status flags for a specified file descriptor.

It takes one command-line argument, which is the file descriptor number. It then
calls `fcntl()` with `F_GETFL` to get the file status flags. The program checks
whether the descriptor is open for reading, writing, or both, and also prints
additional flags such as append mode, nonblocking mode, or synchronous writes.

##### Key concepts

- `fcntl()` can inspect or modify properties of an open file descriptor.
- `F_GETFL` gets the file status flags for a file descriptor.
- `O_RDONLY` means the file is open for reading only.
- `O_WRONLY` means the file is open for writing only.
- `O_RDWR` means the file is open for both reading and writing.
- `O_APPEND` means writes are added to the end of the file.
- `O_NONBLOCK` means operations should not block if data is not ready.
- `O_ACCMODE` is used to extract the access mode bits.

#### Program 3.5 — Turn on file status flags

File: `programs/ch3/5.c`

This program defines a function that turns on one or more file status flags for
an already open file descriptor.

It first calls `fcntl()` with `F_GETFL` to get the current file status flags.
Then it uses the bitwise OR operator to add the requested flags. Finally, it
calls `fcntl()` with `F_SETFL` to store the updated flags back into the file
descriptor.

##### Key concepts

- `F_GETFL` gets the current file status flags.
- `F_SETFL` sets new file status flags.
- Existing flags should be fetched before changing them.
- `val |= flags` turns on selected flags without clearing the old ones.
- `val &= ~flags` can be used to turn off selected flags.
- File status flags include options such as `O_APPEND`, `O_NONBLOCK`, and `O_SYNC`.
- Changing flags incorrectly can accidentally clear flags that were already set.

#### Program 4.1 — Print file type for each command-line argument

File: `programs/ch4/1.c`

This program prints the file type of each pathname given as a command-line
argument.

It calls `lstat()` to get information about each file. The returned information
is stored in a `struct stat` object. The program then checks the `st_mode` field
using macros such as `S_ISREG`, `S_ISDIR`, and `S_ISLNK` to determine the file
type.

It uses `lstat()` instead of `stat()` so that symbolic links are reported as
symbolic links. If `stat()` were used, the program would follow the symbolic
link and report the type of the file it points to.

##### Key concepts

- `lstat()` gets file information without following symbolic links.
- `struct stat` stores metadata about a file.
- `st_mode` contains both file type information and permission bits.
- `S_ISREG()` checks whether the file is a regular file.
- `S_ISDIR()` checks whether the file is a directory.
- `S_ISCHR()` checks whether the file is a character special file.
- `S_ISBLK()` checks whether the file is a block special file.
- `S_ISFIFO()` checks whether the file is a FIFO.
- `S_ISLNK()` checks whether the file is a symbolic link.
- `S_ISSOCK()` checks whether the file is a socket.

#### Program 4.2 — Check file access using `access()` and `open()`

File: `programs/ch4/2.c`

This program demonstrates the difference between checking access permission
with `access()` and actually opening a file with `open()`.

It first calls `access()` with `R_OK` to check whether the real user can read
the file. Then it calls `open()` with `O_RDONLY` to try to open the file for
reading.

This distinction is important for set-user-ID programs. `access()` checks
permissions using the real user ID and real group ID, while `open()` checks
permissions using the effective user ID and effective group ID.

##### Key concepts

- `access()` tests whether a file is accessible.
- `R_OK` checks for read permission.
- `F_OK` checks whether the file exists.
- `W_OK` checks for write permission.
- `X_OK` checks for execute permission.
- `access()` uses the real user ID and real group ID.
- `open()` uses the effective user ID and effective group ID.
- In a set-user-ID program, `access()` can fail while `open()` succeeds.
- This lets a privileged program check what the real user is allowed to do.

#### Program 4.3 — Demonstrate the `umask()` function

File: `programs/ch4/3.c`

This program demonstrates how the file mode creation mask affects the
permissions of newly created files.

It first calls `umask(0)` so that no permission bits are disabled. Then it
creates a file named `foo` with read and write permissions for the owner, group,
and others.

Next, it changes the mask so that group and others permissions are disabled.
It then creates another file named `bar` with the same requested permissions.
Because of the new mask, `bar` is created with permissions only for the owner.

##### Key concepts

- `umask()` sets the file mode creation mask for the current process.
- The file creation mask controls which permission bits are turned off.
- The final file mode is the requested mode with the `umask` bits removed.
- `umask(0)` means no permission bits are disabled.
- `creat()` creates a new file with a requested permission mode.
- `S_IRUSR` means owner read permission.
- `S_IWUSR` means owner write permission.
- `S_IRGRP` and `S_IWGRP` are group read and write permissions.
- `S_IROTH` and `S_IWOTH` are others read and write permissions.
- Changing `umask()` in a child process does not change the parent shell’s `umask`.

#### Program 4.4 — Change file permissions using `chmod()`

File: `programs/ch4/4.c`

This program demonstrates how to change file permission bits with `chmod()`.

For the file `foo`, it first calls `stat()` to obtain the current file mode.
It then turns off the group-execute permission and turns on the set-group-ID
bit while preserving the other permission bits.

For the file `bar`, it sets the permissions directly to `rw-r--r--`,
regardless of the file’s previous permissions.

##### Key concepts

- `chmod()` changes a file’s permission and special mode bits.
- `stat()` retrieves the current file mode through `st_mode`.
- `S_IXGRP` represents group-execute permission.
- `S_ISGID` represents the set-group-ID bit.
- `mode & ~flag` turns off a selected permission bit.
- `mode | flag` turns on a selected permission bit.
- Existing permissions should be retrieved first when only selected bits are changed.
- Passing a complete mode to `chmod()` replaces the previous permission bits.
- `S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH` produces `rw-r--r--`.


#### Program 4.5 — Open a file and then unlink it

File: `programs/ch4/5.c`

This program opens a file named `tempfile` and then removes its directory
entry with `unlink()`.

Although the filename disappears immediately, the file itself is not fully
deleted because it is still open. The program waits for 15 seconds before
exiting, allowing the user to observe that the file cannot be found by name
but still occupies disk space.

When the process exits, the kernel closes the open file descriptor. If no
other links or open descriptors remain, the file’s data and disk space are
finally released.

##### Key concepts

- `open()` opens a file and creates an open file descriptor.
- `unlink()` removes a filename from its directory.
- Removing a directory entry does not always delete the file immediately.
- An open file remains accessible through its file descriptor.
- Disk space is released only when the link count is zero and no process has the file open.
- `sleep()` keeps the process running so the file state can be observed.
- Exiting the process automatically closes its open file descriptors.
- Programs often unlink temporary files immediately after opening them.
- This technique allows temporary files to be cleaned up automatically when a process exits.


#### Program 4.6 — Truncate files while preserving their timestamps

File: `programs/ch4/6.c`

This program truncates each file given as a command-line argument to zero
bytes while preserving its original access time and modification time.

For each file, it first calls `stat()` to save the current timestamps. It then
opens the file with `O_TRUNC`, which removes all file contents. Finally, it
calls `utime()` to restore the saved access and modification times.

##### Key concepts

- `stat()` retrieves a file’s metadata and timestamps.
- `st_atime` stores the file’s last access time.
- `st_mtime` stores the file’s last modification time.
- `O_TRUNC` reduces an existing file’s size to zero.
- `O_TRUNC` requires the file to be opened with write access.
- Truncating a file normally changes its modification time.
- `struct utimbuf` stores a new access time and modification time.
- `utime()` changes a file’s access and modification timestamps.
- The file contents are removed, but the original timestamps are restored.
- The program processes every filename supplied through `argv`.