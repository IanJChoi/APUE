# APUE Study Notes

My notes and practice programs while studying *Advanced Programming in the UNIX Environment*.

## Programs

### Chapter 1

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

### Chapter 2

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
