#ifndef CURL_H
#define CURL_H

#include <curl/curl.h>
#include <string>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <iostream>

class CurlDownload {
    CURL *curl = nullptr;
public:
    CurlDownload() {
        curl = curl_easy_init();
    }
    ~CurlDownload() {
        if (curl) {
            curl_easy_cleanup(curl);
        }
    }
    static bool globalInit() {
        return curl_global_init(CURL_GLOBAL_DEFAULT) == CURLE_OK;
    }
    static bool globalCleanup() {
        curl_global_cleanup();
        return true;
    }
    static size_t fileWriteCallBack(void *ptr, size_t size, size_t nmemb, FILE *stream) {
        if (stream) {
            return fwrite(ptr, size, nmemb, stream);
        }
        return 0;
    }
    bool download(const std::string &url, const std::string &outputFile) {
        if (!curl) {
            // CURL is not initialized
            return false;
        }
        FILE *fp = fopen(outputFile.c_str(), "wb");
        if (!fp) {
            // Failed to open file for writing
            return false;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fileWriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        bool result = (curl_easy_perform(curl) == CURLE_OK);
        fclose(fp);
        return result;
    }
    std::string getLastError() const {
        if (curl) {
            return curl_easy_strerror(curl_easy_perform(curl));
        }
        return "CURL not initialized";
    }
};

#endif // CURL_H
