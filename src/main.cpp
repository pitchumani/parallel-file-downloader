#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <stdexcept>

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
    // Download each URL in the queue
    while (!urls.empty()) {
        std::string url = urls.front();
        urls.pop();
        std::string outputFile = basename(url);

        std::cout << "Downloading: " << url << " to '" << outputFile << "'" << std::endl;
        if (!downloader.download(url, outputFile)) {
            std::cerr << "Failed to download: " << url << std::endl;
        }
    }

    return 0;
}

