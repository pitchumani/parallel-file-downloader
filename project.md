# Project Idea: Parallel File Downloader

The goal is to create a command-line application that downloads multiple files from a list of URLs concurrently, rather than one by one. This will significantly speed up the total download time.

## Core Concepts & Implementation Overview
### 1. The Task Queue 📥
You'll start with a list of URLs to download. These URLs will act as your "tasks." You need a thread-safe queue to hold these tasks. A std::queue is a good starting point.

### 2. The Thread Pool 🏊
Instead of creating a new thread for every single download (which is inefficient), you'll create a fixed number of worker threads at the start—this is your thread pool. These threads will run in the background, waiting for tasks to appear in the queue.

Implementation: You can use a std::vector<std::thread> to hold your worker threads.
### 3. Mutexes for Thread Safety 🔒
Multiple threads will be trying to access the task queue at the same time. One thread will be trying to pull a URL from the queue while another might be doing the same. This is a classic race condition.

To prevent this, you'll use a std::mutex. Before a thread can access the queue, it must "lock" the mutex. Once it's done, it "unlocks" it, allowing other threads to access the queue. This ensures that only one thread can modify the queue at any given time.

Implementation: You'll pair your std::queue with a std::mutex. A std::lock_guard is a convenient way to manage the locking and unlocking automatically.
### 4. Condition Variables for Efficiency 🚦
If the task queue is empty, your worker threads will be constantly locking and unlocking the mutex just to check for work, which wastes CPU cycles. This is called "busy-waiting."

A std::condition_variable solves this problem. It allows threads to sleep until there's actual work to be done.

How it works:
A worker thread locks the mutex and checks the queue.
If the queue is empty, the thread calls wait() on the condition variable. This atomically unlocks the mutex and puts the thread to sleep.
When the main thread adds new URLs to the queue, it will lock the mutex, add the work, and then call notify_one() or notify_all() on the condition variable.
This wakes up one or all of the sleeping threads. The awakened thread then re-acquires the lock and checks the queue again.

## Project Workflow
### 1. Initialization:
- The main thread populates a global, thread-safe queue with the URLs to be downloaded.
- It then creates and starts a pool of worker threads.
### 2. Worker Thread Logic (the main loop):
- Each worker thread enters a loop.
- Inside the loop, it locks the mutex to gain exclusive access to the task queue.
- It uses a condition variable to wait until the queue is not empty.
- Once awakened and with the lock acquired, it dequeues a URL.
- It unlocks the mutex so other threads can access the queue.
- It proceeds to download the file associated with the URL (you can use a library like cpr or libcurl for this).
- The loop continues until all tasks are done.
### 3. Shutdown:
- Once all URLs are added, the main thread needs a way to signal to the worker threads that no more tasks will be added. A simple boolean flag (also protected by the mutex) can work.
- The main thread then waits for all worker threads to finish their current tasks and exit their loops using thread.join().
