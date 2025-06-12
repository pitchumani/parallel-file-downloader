#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <curl/curl.h>


int main (int argc, char *argv[]) {
    std::cout << "Parallel Downloading Example" << std::endl;

    // list of URLs to download
    std::queue<std::string> urls;
    urls.push("https://ftp.gnu.org/gnu/bash/bash-2.01.tar.gz");
    urls.push("https://ftp.gnu.org/gnu/bash/bash-2.02.tar.gz");
    urls.push("https://ftp.gnu.org/gnu/bash/bash-2.03.tar.gz");
    urls.push("https://ftp.gnu.org/gnu/bash/bash-2.04.tar.gz");
    urls.push("https://ftp.gnu.org/gnu/bash/bash-2.05.tar.gz");

    while (!urls.empty()) {
        std::string url = urls.front();
        urls.pop();
        std::cout << "Downloading: " << url << std::endl;
    }

    return 0;
}

