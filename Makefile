# Makefile for parallel-file-downloader
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lcurl
GTEST_LDFLAGS = -lgtest -lgtest_main -lcurl

SRC_DIR = $(realpath ./src)
BUILD_DIR = $(SRC_DIR)/../build
SRC = $(SRC_DIR)/main.cpp
HDRS = $(SRC_DIR)/curl-download.h \
	   $(SRC_DIR)/thread-pool.h

TARGET = $(BUILD_DIR)/pfd
GTEST_DIR = $(realpath ./gtest)
GTEST_SRC = $(wildcard $(GTEST_DIR)/*.cpp)

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(SRC) $(HDRS)| $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC) $(LDFLAGS)

.PHONY: clean gtest

clean:
	rm -rf $(BUILD_DIR)

gtest: $(GTEST_SRC) | $(BUILD_DIR)
	@echo "Building and running Google Test (gtest) target..."
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/gtest $(GTEST_SRC) $(GTEST_LDFLAGS)
	cd build && ./gtest

example1: $(SRC_DIR)/thread-example1.cpp | $(BUILD_DIR)
	@echo "Building and running example1..."
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/example1 $(SRC_DIR)/thread-example1.cpp $(LDFLAGS)
	@echo "Running example1..."
	@cd $(BUILD_DIR) && ./example1

example2: $(SRC_DIR)/thread-example2.cpp | $(BUILD_DIR)
	@echo "Building and running example2..."
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/example2 $(SRC_DIR)/thread-example2.cpp $(LDFLAGS)
	@echo "Running example2..."
	@cd $(BUILD_DIR) && ./example2

examples: example1 example2 | $(BUILD_DIR)
	@echo "All examples built and run."
