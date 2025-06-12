# Makefile for parallel-file-downloader
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC_DIR = $(realpath ./src)
BUILD_DIR = $(SRC_DIR)/../build
SRC = $(SRC_DIR)/main.cpp
TARGET = $(BUILD_DIR)/pfd
GTEST_DIR = $(realpath ./gtest)
GTEST_SRC = $(wildcard $(GTEST_DIR)/*.cpp)

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

.PHONY: clean gtest

clean:
	rm -rf $(BUILD_DIR)

gtest: $(GTEST_SRC) | $(BUILD_DIR)
	@echo "Building and running Google Test (gtest) target..."
	$(CXX) $(CXXFLAGS) -pthread -lgtest -lgtest_main -o $(BUILD_DIR)/gtest $(GTEST_SRC)
	$(BUILD_DIR)/gtest
