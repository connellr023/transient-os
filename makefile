SRC_DIR_NAME = src
BUILD_DIR_NAME = build

SRC_DIR = ./$(SRC_DIR_NAME)
BUILD_DIR = ./$(BUILD_DIR_NAME)

# Find all .c and .cpp files recursively in the source directory
SRCS = $(shell find $(SRC_DIR) -type f \( -name '*.c' -o -name '*.cpp' \))
OBJS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SRCS:.c=.o))
OBJS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(OBJS:.cpp=.o))

all: clean $(BUILD_DIR)/kernel8.img

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/kernel8.img: $(BUILD_DIR)/start.o $(OBJS)
	ld.lld -m aarch64elf -nostdlib $(BUILD_DIR)/start.o $(OBJS) -T link.ld -o $(BUILD_DIR)/kernel8.elf
	llvm-objcopy -O binary $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/kernel8.img

$(BUILD_DIR)/start.o: $(SRC_DIR)/start.s | $(BUILD_DIR)
	clang --target=aarch64-elf $(CFLAGS) -c $(SRC_DIR)/start.s -o $(BUILD_DIR)/start.o

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	clang --target=aarch64-elf $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	clang++ --target=aarch64-elf $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR_NAME)