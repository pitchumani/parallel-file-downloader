#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <stdexcept>

#include "thread-pool.h"
#include "curl-download.h"

// Function to extract the basename from a given path
std::string basename(const std::string &path) {
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos) {
        return path; // No directory part, return the whole path
    }
    return path.substr(pos + 1); // Return the part after the last '/'
}

int main (int, char **) {
    std::cout << "Parallel Downloading Example" << std::endl;

    // list of URLs to download
    std::queue<std::string> urls;
    urls.push("https://ftp.gnu.org/gnu/bash/bash-2.01.tar.gz");
    urls.push("https://ftp.gnu.org/gnu/bash/bash-2.02.tar.gz");
    urls.push("https://ftp.gnu.org/gnu/bash/bash-2.03.tar.gz");
    urls.push("https://ftp.gnu.org/gnu/bash/bash-2.04.tar.gz");
    urls.push("https://ftp.gnu.org/gnu/bash/bash-2.05.tar.gz");

    // Initialize CURL globally
    CurlDownload::globalInit();
    // Create a downloader instance
    CurlDownload downloader;
    std::mutex mtx;
    int numThreads = 2;

    // define the worker function
    std::function<void()> downloadTask = [&downloader, &urls, &mtx]() {
        while (true) {
            std::string url;
            {
                // The scope of the lock is limited to this block, ensuring that the
                // lock is released as soon as we are done accessing the shared resource.

                // acquire a lock to access the resource urls
                std::lock_guard<std::mutex> lock(mtx);
                if (urls.empty()) break;
                url = urls.front();
                urls.pop();
            }
            if (url.empty()) {
                std::cerr << "Empty URL encountered, skipping." << std::endl;
                continue;
            }
            std::cout << "URL: " << url << std::endl;
            std::string outputFile = basename(url);
            if (outputFile.empty()) {
                std::cerr << "Could not derive output filename from URL, skipping." << std::endl;
                continue;
            }
            std::cout << "Thread id (" << std::this_thread::get_id() << "): "
                      << " downloading to " << outputFile << ": ";
            auto result = downloader.download(url, outputFile);
            if (!result) {
                std::cout << "FAILED" << std::endl;
            } else {
                std::cout << "SUCCESS" << std::endl;
            }
        }
    };

    try {
        // create the thread pool with the task to be executed
        ThreadPool threadPool(numThreads, downloadTask);

        // Wait for all tasks to complete
        threadPool.waitForCompletion();
        std::cout << "All tasks are completed." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error occurred: " << e.what() << std::endl;
    }

    // Cleanup CURL globally
    CurlDownload::globalCleanup();
    return 0;
}

