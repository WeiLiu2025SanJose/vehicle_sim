CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pthread -Iinc

SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
TEST_DIR := tests

# Collect sources and objects
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Exclude main.o for test builds
OBJS_NO_MAIN := $(filter-out $(BUILD_DIR)/main.o,$(OBJS))

# Test source files
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/$(TEST_DIR)/%.o,$(TEST_SRCS))

# Default target: build main app
all: $(BIN_DIR)/simulation

# Main application binary
$(BIN_DIR)/simulation: $(OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "Linking simulation binary..."
	$(CXX) $(CXXFLAGS) -o $@ $^

# ---- ENABLE UNIT TESTING FOR test TARGET ----
test: CXXFLAGS += -DUNIT_TESTING
# Test runner binary
test: $(BIN_DIR)/test_runner

$(BIN_DIR)/test_runner: $(OBJS_NO_MAIN) $(TEST_OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "Linking test runner..."
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cpp files from src/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile .cpp files from tests/
$(BUILD_DIR)/$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean all build and binary files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean test
