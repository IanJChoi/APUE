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
