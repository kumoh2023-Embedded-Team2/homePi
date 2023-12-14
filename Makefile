CC := gcc
CFLAGS := -Wall -Wextra -Iinclude
LDFLAGS := -lwiringPi -lpthread

SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

# Find all source files in the source directory
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Generate the corresponding object file names
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# The final executable name
TARGET := homePi

# Build rule for object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c 
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build rule for the final executable
$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Phony target to clean the build
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
