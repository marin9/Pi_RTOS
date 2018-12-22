CC=arm-none-eabi
CFLAGS=-mcpu=arm926ej-s -ffreestanding -nostdlib -O2 -Wall -Wextra -c -I$(INCLUDE_DIR)
LDFLAGS=-ffreestanding -nostdlib -lgcc

ARCH_DIR=arch
KERNEL_DIR=kernel
PROGRAM_DIR=prog
INCLUDE_DIR=include
OBJ_DIR=obj
ELF=kernel.elf
TARGET=kernel.img
LINKER=$(ARCH_DIR)/linker.ld

SRC_ARCHS	:= $(wildcard $(ARCH_DIR)/*.s)
SRC_ARCHC   := $(wildcard $(ARCH_DIR)/*.c)
SRC_KERNELC	:= $(wildcard $(KERNEL_DIR)/*.c)
SRC_PROGC	:= $(wildcard $(PROGRAM_DIR)/*.c)

OBJ_ARCHS	:= $(SRC_ARCHS:$(ARCH_DIR)/%.s=$(OBJ_DIR)/%.o)
OBJ_ARCHC   := $(SRC_ARCHC:$(ARCH_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_KERNELC	:= $(SRC_KERNELC:$(KERNEL_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_PROGC	:= $(SRC_PROGC:$(PROGRAM_DIR)/%.c=$(OBJ_DIR)/%.o)

INCLUDES := $(wildcard $(INCLUDE_DIR)/*.h)



all: $(TARGET)
	@echo $(ARCH)

run: $(TARGET)
	@echo "Starting qemu (Exit: Ctrl+A then X)"
	@qemu-system-arm -M versatilepb -m 128M -nographic -kernel $(TARGET)

gdb: $(TARGET)
	@echo "Starting qemu (Exit: Ctrl+A then X)"
	@qemu-system-arm -M versatilepb -m 128M -nographic -kernel $(TARGET) -s -S

clean:
	@-rm -rf $(OBJ_DIR)
	@-rm -f $(TARGET)
	@echo "Clean"


$(TARGET): $(OBJ_DIR) $(OBJ_DIR)/$(ELF) $(INCLUDE_DIR)
	@echo "Create  $(TARGET)"
	@$(CC)-objcopy $(OBJ_DIR)/$(ELF) -O binary $(TARGET)

$(OBJ_DIR)/$(ELF): $(LINKER) $(OBJ_ARCHC) $(OBJ_ARCHS) \
				   $(OBJ_LIBC) $(OBJ_KERNELC) $(OBJ_PROGC)
	@echo "Link files"
	@$(CC)-gcc -T $(LINKER) $(OBJ_ARCHC) $(OBJ_ARCHS) \
				  $(OBJ_LIBC) $(OBJ_KERNELC) $(OBJ_PROGC) \
				  -o $@ $(LDFLAGS)


$(OBJ_ARCHC): $(OBJ_DIR)/%.o : $(ARCH_DIR)/%.c $(INCLUDES)
	@echo Compile $<
	@$(CC)-gcc $(CFLAGS) $< -o $@

$(OBJ_ARCHS): $(OBJ_DIR)/%.o : $(ARCH_DIR)/%.s $(INCLUDES)
	@echo Compile $<
	@$(CC)-gcc $(CFLAGS) $< -o $@


$(OBJ_KERNELC): $(OBJ_DIR)/%.o : $(KERNEL_DIR)/%.c $(INCLUDES)
	@echo Compile $<
	@$(CC)-gcc $(CFLAGS) $< -o $@

$(OBJ_PROGC): $(OBJ_DIR)/%.o : $(PROGRAM_DIR)/%.c $(INCLUDES)
	@echo Compile $<
	@$(CC)-gcc $(CFLAGS) $< -o $@

$(OBJ_DIR):
	@mkdir $@
