# parallel-file-downloader
A console application to download multiple files from URLs concurrently. This application uses a thread pool to manage threads efficiently and is created to learn about threads, thread-pool, and mutexes.

## Examples
### Example1
Refer `src/thread-example1.cpp`
This example demonstrates race conditions that occur when multiple threads access shared memory concurrently.

### Example2
Refer `src/thread-example2.cpp`
This example demonstrates how the race conditions (from example1) are solved
by using a mutex to ensure thread-safe access to shared resources, preventing race conditions.

> **Mutex:**
A mechanism to prevent race conditions to ensure the integrity of the shared
resources in the multi threaded environment.
When a thread wants to access a shared resource protected by mutexes, it should attempt to lock
the mutex. If the mutex is already locked (by another thread), then the requesting thread is
blocked until the mutex is unlocked.

> **Deadlock:**
The mutexes prevent race conditions, they can also lead to deadlock if not used carefully. Deadlock occurs when two or more threads are blocked indefinitely, each waiting for other to release a  resource.
