CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -mcpu=cortex-a53+nosimd

SRC_DIR_NAME = src
BUILD_DIR_NAME = build

SRC_DIR = ./$(SRC_DIR_NAME)
BUILD_DIR = ./$(BUILD_DIR_NAME)

all: clean $(BUILD_DIR)/kernel8.img

$(BUILD_DIR):
	mkdir $(BUILD_DIR_NAME)

$(BUILD_DIR)/kernel8.img: $(BUILD_DIR)/start.o
	ld.lld -m aarch64elf -nostdlib $(BUILD_DIR)/start.o -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf
	llvm-objcopy -O binary $(BUILD_DIR)/kernel8.elf $(BUILD_DIR)/kernel8.img

$(BUILD_DIR)/start.o: $(SRC_DIR)/start.s | $(BUILD_DIR)
	clang --target=aarch64-elf $(CFLAGS) -c $(SRC_DIR)/start.s -o $(BUILD_DIR)/start.o

clean:
	@if exist $(BUILD_DIR)\\kernel8.elf del /Q $(BUILD_DIR)\\kernel8.elf
	@if exist $(BUILD_DIR)\\*.o del /Q $(BUILD_DIR)\\*.o