# Multithreaded Linked List Operations Performance Testing with RWLock and Mutex

This project implements performance testing for multithreaded linked list operations (`Insert`, `Member`, and `Delete`) using **Read-Write Locks (RWLock)** and **Mutex** for synchronization. The program allows you to compare the performance of these synchronization mechanisms under different workloads.

## Features

- **Thread-safe linked list operations**: Insert, Member, and Delete operations are performed safely in a multithreaded environment using synchronization mechanisms.
- **Read-Write Lock**: Efficient for read-heavy workloads, allowing multiple readers but exclusive writers.
- **Mutex**: Simple exclusive lock that serializes all operations.
- **Dynamic iteration calculation**: Optionally calculate the minimum number of iterations to meet a desired accuracy.
- **Progress output**: Results are printed both to the console and a file (`results.txt`).

## Requirements

### Dependencies

- **C Compiler**:
  - **Windows**: `MinGW` or any compiler with POSIX threading support (via `pthreads` library) (compiled and tested on a windows machine with MinGW)
- **POSIX Threads (pthread)**: Available on most Unix-based systems such as Linux or macOS. For Windows, install pthreads using MinGW or another POSIX-compatible environment.

## Compilation

### Windows (compiled and tested on a windows machine)

To compile the program on Windows, run the following command:

```bash
gcc  -o performance_test main.c rw_lock.c mutex.c serial.c linked_list.c
```
To run the program on Windows, run the following command:

```bash
./performance_test
```
Or just run the pre-compiled program on Windows, using this command::

```bash
./performance_test_compiled
