// This example demonstrates a multiple thread execution scenario where
// multiple threads increment a shared counter.

#include <thread>
#include <iostream>
#include <unistd.h> // For sleep function
#include <cstdlib> // For rand function

// Simple global counter class to demonstrate shared state 
class GlobalCounter {
public:
    int value = 0; // Shared value to be incremented by threads
    void increment() {
        // Simulate a workload that exposes race conditions
        int temp = value; // Read shared value
        // Simulate computation
        for (volatile int i = 0; i < 1000000; ++i) { /* busy wait */ }
        temp += 1; // Increment local copy
        value = temp; // Write back to shared value

        std::cout << "Counter incremented to: " << value << " by thread ID: "
                  << std::this_thread::get_id() << std::endl;
    }
    GlobalCounter() = default; // Default constructor
    ~GlobalCounter() = default; // Default destructor
    GlobalCounter(const GlobalCounter&) = delete; // Disable copy constructor
    GlobalCounter& operator=(const GlobalCounter&) = delete; // Disable assignment operator
    GlobalCounter(GlobalCounter&&) = delete; // Disable move constructor
    GlobalCounter& operator=(GlobalCounter&&) = delete; // Disable move assignment operator
};

int main() {
    std::cout << "Starting threads to increment global counter..." << std::endl;

    // create a global counter instance
    GlobalCounter *globalCounter = new GlobalCounter();
    // function to call increment on the global counter
    auto incrementFunc = [globalCounter]() {
        globalCounter->increment(); // Increment the global counter
    };

    std::cout << "Initial value of global counter: " << globalCounter->value << std::endl;

    // create 4 threads to increment the global counter
    std::thread t1(incrementFunc);
    std::thread t2(incrementFunc);
    std::thread t3(incrementFunc);
    std::thread t4(incrementFunc);
    // wait for all threads to finish
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    // print the final value of the global counter
    std::cout << "Final value of global counter: " << globalCounter->value << std::endl;
    std::cout << "If the final value is not equal to 4, it indicates a race condition occurred." << std::endl;
    // clean up
    delete globalCounter; // Delete the global counter instance
    return 0; // Return success
}
