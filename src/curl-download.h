#ifndef CURL_H
#define CURL_H

#include <curl/curl.h>
#include <string>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <iostream>

class CurlDownload {
    CURLcode lastErrorCode = CURLE_OK;

public:
    CurlDownload() = default;
    ~CurlDownload() = default;

    static size_t fileWriteCallBack(void *ptr, size_t size, size_t nmemb, FILE *stream) {
        if (stream) {
            return fwrite(ptr, size, nmemb, stream);
        }
        return 0;
    }
    bool download(CURL *curl, const std::string &url, const std::string &outputFile) {
        if (!curl) {
            lastErrorCode = CURLE_FAILED_INIT;
            std::cerr << "CURL error: " << curl_easy_strerror(lastErrorCode) << std::endl;
            return false;
        }
        FILE *fp = fopen(outputFile.c_str(), "wb");
        if (!fp) {
            lastErrorCode = CURLE_WRITE_ERROR;
            std::cerr << "CURL error: " << curl_easy_strerror(lastErrorCode) << std::endl;
            return false;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fileWriteCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        lastErrorCode = curl_easy_perform(curl);
        fclose(fp);
        if (lastErrorCode != CURLE_OK) {
            std::cerr << "CURL error: " << curl_easy_strerror(lastErrorCode) << std::endl;
            return false;
        }
        return true;
    }
    std::string getLastError() const {        
        return curl_easy_strerror(lastErrorCode);
    }
};

#endif // CURL_H
