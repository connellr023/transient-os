SRC_DIR_NAME = src
BUILD_DIR_NAME = build
INCLUDE_DIR_NAME = include

SRC_DIR = ./$(SRC_DIR_NAME)
BUILD_DIR = ./$(BUILD_DIR_NAME)
INCLUDE_DIR = ./$(INCLUDE_DIR_NAME)

# Find all .c, .cpp, and .s files recursively in the source directory
SRCS = $(shell find $(SRC_DIR) -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.s' \))
OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRCS:.c=.o))
OBJS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(OBJS:.cpp=.o))
OBJS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(OBJS:.s=.o))

# Compiler and linker flags
CFLAGS = -fno-exceptions -fno-rtti -Wall -Wextra -I$(INCLUDE_DIR)
CXXFLAGS = $(CFLAGS)
LDFLAGS = -nostartfiles -ffreestanding -lgcc

# Output library name
LIB_NAME = libtransient-os.a

# Default target
all: $(BUILD_DIR)/$(LIB_NAME)

# Build the static library
$(BUILD_DIR)/$(LIB_NAME): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	llvm-ar rcs $@ $^

# Compile C source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $< to $@"
	clang++ --target=aarch64-elf $(CFLAGS) -c $< -o $@

# Compile C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $< to $@"
	clang++ --target=aarch64-elf $(CXXFLAGS) -c $< -o $@

# Compile assembly source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $< to $@"
	clang++ --target=aarch64-elf -x assembler-with-cpp $(CFLAGS) -c $< -o $@

# Ensure build directory exists
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean