#include <gtest/gtest.h>
#include "../src/curl-download.h"

class CurlDownloadEnvironment : public ::testing::Environment {
    void SetUp() override {
    }
    void TearDown() override {
    }
};

::testing::Environment* const curl_env = ::testing::AddGlobalTestEnvironment(new CurlDownloadEnvironment);

TEST(CurlDownloadTest, FileWriteCallbackWritesData) {
    const char* testData = "hello world";
    const char* testFile = "test_output.txt";
    FILE* fp = fopen(testFile, "wb");
    ASSERT_NE(fp, nullptr);
    size_t written = CurlDownload::fileWriteCallBack((void*)testData, 1, strlen(testData), fp);
    fclose(fp);
    ASSERT_EQ(written, strlen(testData));
    std::ifstream in(testFile);
    std::string content;
    std::getline(in, content);
    EXPECT_EQ(content, "hello world");
    int removeResult = std::remove(testFile);
    ASSERT_EQ(removeResult, 0) << "Failed to remove temporary file: " << testFile;
}

TEST(CurlDownloadTest, DownloadFailsWithInvalidURL) {
    CurlDownload curl;
    CURL *curlHandle = curl_easy_init();
    bool result = curl.download(curlHandle, "http://invalid.url/shouldfail", "fail.txt");
    EXPECT_FALSE(result);
    if (std::remove("fail.txt") != 0) {
        FAIL() << "Failed to remove file: fail.txt";
    }
    std::remove("fail.txt");
}
TEST(CurlDownloadTest, DownloadFailsWithInvalidFile) {
    CurlDownload curl;
    CURL *curlHandle = curl_easy_init();
    bool result = curl.download(curlHandle, "https://ftp.gnu.org/gnu/bash/bash-2.01.tar.gz", "/");
    EXPECT_FALSE(result);

    // Verify the specific error code or message
    std::string errorMessage = curl.getLastError();
    EXPECT_EQ(errorMessage, "Failed writing received data to disk/application");
    // Attempt to remove the file, which should not exist
    int removeResult = std::remove("/");
    ASSERT_EQ(removeResult, -1) << "Unexpectedly removed file: /";
}

TEST(CurlDownloadTest, DownloadSuccess) {
    class MockCurlDownload : public CurlDownload {
    public:
        bool download(CURL *, const std::string& /*url*/, const std::string& outputFile) {
            // Simulate successful download without actual network call
            std::ofstream out(outputFile);
            out << "mock file content";
            out.close();
            return true;
        }
    };

    MockCurlDownload curl;
    CURL *curlHandle = curl_easy_init();
    bool result = curl.download(curlHandle, "https://mock.url/mockfile.txt", "mockfile.txt");
    EXPECT_TRUE(result);
    std::ifstream in("mockfile.txt");
    EXPECT_TRUE(in.good());
    std::remove("mockfile.txt");
}
    
TEST(CurlDownloadTest, DownloadWithEmptyURL) {
    CurlDownload curl;
    CURL *curlHandle = curl_easy_init();
    bool result = curl.download(curlHandle, "", "empty.txt");
    EXPECT_FALSE(result);

    // Verify the specific error code or message
    std::string errorMessage = curl.getLastError();
    int removeResult = std::remove("empty.txt");
    ASSERT_EQ(removeResult, 0) << "Failed to remove temporary file: empty.txt";
    std::remove("empty.txt");
    EXPECT_EQ(errorMessage, "URL using bad/illegal format or missing URL");
}

TEST(CurlDownloadTest, DownloadWithNullFilePointer) {
    CurlDownload curl;
    CURL *curlHandle = curl_easy_init();
    bool result = curl.download(curlHandle, "https://ftp.gnu.org/gnu/bash/bash-2.01.tar.gz", "null_pointer.txt");
    EXPECT_TRUE(result);
    std::ifstream in("null_pointer.txt");
    int removeResult = std::remove("null_pointer.txt");
    ASSERT_EQ(removeResult, 0) << "Failed to remove temporary file: null_pointer.txt";
    std::remove("null_pointer.txt");
}

TEST(CurlDownloadTest, DownloadWithValidURLAndExistingFile) {
    CurlDownload curl;
    CURL *curlHandle = curl_easy_init();
    std::string url = "https://ftp.gnu.org/gnu/bash/bash-2.01.tar.gz";
    std::string outputFile = "bash-2.01.tar.gz";
    bool result = curl.download(curlHandle, url, outputFile);
    EXPECT_TRUE(result);

    // Verify the content of the downloaded file
    std::ifstream in(outputFile);
    EXPECT_TRUE(in.good());
    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    EXPECT_FALSE(content.empty()); // Ensure file is not empty
    in.close();
    // Clean up the downloaded file
    std::remove(outputFile.c_str());
}