CC=arm-none-eabi
CFLAGS=-mcpu=arm926ej-s -ffreestanding -nostdlib -O2 -Wall -Wextra -c
LDFLAGS=-ffreestanding -nostdlib -lgcc

ARCH=arch
DRIVERS=drivers
INCLUDE=include
KERNEL=kernel
PROG=prog
LIB=lib
OBJ=obj
ELF=kernel.elf
TARGET=kernel.img
LINKER=$(ARCH)/linker.ld

SRC_ARCHS	:= $(wildcard $(ARCH)/*.s)
SRC_ARCHC   := $(wildcard $(ARCH)/*.c)
SRC_DRIVERS	:= $(wildcard $(DRIVERS)/*.c)
SRC_INCLUDE	:= $(wildcard $(INCLUDE)/*.h)
SRC_KERNEL	:= $(wildcard $(KERNEL)/*.c)
SRC_PROG	:= $(wildcard $(PROG)/*.c)
SRC_LIB		:= $(wildcard $(LIB)/*.c)

OBJ_ARCHS	:= $(SRC_ARCHS:$(ARCH)/%.s=$(OBJ)/%.o)
OBJ_ARCHC   := $(SRC_ARCHC:$(ARCH)/%.c=$(OBJ)/%.o)
OBJ_DRIVERS	:= $(SRC_DRIVERS:$(DRIVERS)/%.c=$(OBJ)/%.o)
OBJ_KERNEL	:= $(SRC_KERNEL:$(KERNEL)/%.c=$(OBJ)/%.o)
OBJ_PROG	:= $(SRC_PROG:$(PROG)/%.c=$(OBJ)/%.o)
OBJ_LIB		:= $(SRC_LIB:$(LIB)/%.c=$(OBJ)/%.o)


all: $(TARGET)


run: $(TARGET)
	@echo "Starting qemu (Exit: Ctrl+A then X)"
	@qemu-system-arm -M versatilepb -m 16M -nographic -kernel $(TARGET)

clean:
	@echo "Clean"
	@-rm -rf $(OBJ)
	@-rm -f $(ELF)
	@-rm -f $(TARGET)


$(TARGET): $(OBJ) $(ELF) $(SRC_INCLUDE)
	@echo " CC\t" $(TARGET)
	@$(CC)-objcopy $(ELF) -O binary $(TARGET)

$(ELF): $(LINKER) $(OBJ_ARCHS) $(OBJ_ARCHC) $(OBJ_DRIVERS) $(OBJ_KERNEL) $(OBJ_LIB)
	@echo " LINK"
	@$(CC)-gcc -T $(LINKER) $(OBJ_ARCHC) $(OBJ_ARCHS) $(OBJ_DRIVERS) $(OBJ_KERNEL) $(OBJ_LIB)\
				-o $@ $(LDFLAGS)


$(OBJ_ARCHS): $(OBJ)/%.o : $(ARCH)/%.s
	@echo " CC\t" $<
	@$(CC)-gcc $(CFLAGS) $< -o $@

$(OBJ_ARCHC): $(OBJ)/%.o : $(ARCH)/%.c $(INCLUDE)
	@echo " CC\t" $<
	@$(CC)-gcc $(CFLAGS) -I$(INCLUDE) $< -o $@

$(OBJ_DRIVERS): $(OBJ)/%.o : $(DRIVERS)/%.c $(INCLUDE)
	@echo " CC\t" $<
	@$(CC)-gcc $(CFLAGS) -I$(INCLUDE) $< -o $@

$(OBJ_KERNEL): $(OBJ)/%.o : $(KERNEL)/%.c $(INCLUDE)
	@echo " CC\t" $<
	@$(CC)-gcc $(CFLAGS) -I$(INCLUDE) $< -o $@

$(OBJ_PROG): $(OBJ)/%.o : $(PROG)/%.c $(INCLUDE)
	@echo " CC\t" $<
	@$(CC)-gcc $(CFLAGS) -I$(INCLUDE) $< -o $@

$(OBJ_LIB): $(OBJ)/%.o : $(LIB)/%.c $(INCLUDE)
	@echo " CC\t" $<
	@$(CC)-gcc $(CFLAGS) -I$(INCLUDE) $< -o $@

$(OBJ):
	@echo " MK\t" $(OBJ)
	@mkdir $@
