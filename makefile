# Ensure build directory exists
BUILD_DIR := build

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compiler and linker settings
CXX := aarch64-linux-gnu-g++
CXXFLAGS := -fno-exceptions -fno-rtti -Wall -Wextra -fno-stack-protector
ASFLAGS :=
LDFLAGS := -T link.ld
LINKFILE := link.ld

# Dynamically find source and include directories
SRC_FILES := $(shell find . -name '*.cpp')
ASM_FILES := $(shell find . -name '*.S')
OBJ_FILES := $(patsubst ./%, $(BUILD_DIR)/%, $(SRC_FILES:.cpp=.o) $(ASM_FILES:.S=.o))

# Include directories
INCLUDE_FLAGS := -I.

# Output executable
TARGET := $(BUILD_DIR)/kernel8.img

# Default target
all: $(TARGET)

# Ensure build directory structure matches source directories
build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

build/%.o: %.S
	@mkdir -p $(dir $@)
	$(CXX) $(ASFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

# Link object files into ELF executable
$(TARGET): $(OBJ_FILES)
	ld.lld -m aarch64elf $(OBJ_FILES) -T $(LINKFILE) -o $(BUILD_DIR)/kernel8.elf
	llvm-objcopy -O binary $(BUILD_DIR)/kernel8.elf $(TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
