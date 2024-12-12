SRC_DIR_NAME = src
BUILD_DIR_NAME = build

SRC_DIR = ./$(SRC_DIR_NAME)
BUILD_DIR = ./$(BUILD_DIR_NAME)

# Find all .c, .cpp, and .s files recursively in the source directory
SRCS = $(shell find $(SRC_DIR) -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.s' \))
OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRCS:.c=.o))
OBJS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(OBJS:.cpp=.o))
OBJS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(OBJS:.s=.o))

# Compiler and linker flags
CFLAGS = -fno-exceptions -fno-rtti -Wall -Wextra -Werror
LDFLAGS = -nostdlib

all: $(BUILD_DIR)/kernel8.img

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/kernel8.img: $(OBJS)
	ld.lld -m aarch64elf $(LDFLAGS) $(OBJS) -T link.ld -o $(BUILD_DIR)/kernel8.elf
	llvm-objcopy -O binary $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/kernel8.img

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $< to $@"
	clang++ --target=aarch64-elf $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $< to $@"
	clang++ --target=aarch64-elf $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $< to $@"
	clang++ --target=aarch64-elf -x assembler-with-cpp $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR_NAME)