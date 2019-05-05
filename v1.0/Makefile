CC=arm-none-eabi
CFLAGS=-mcpu=arm926ej-s -ffreestanding -nostdlib -O2 -Wall -Wextra -c
LDFLAGS=-ffreestanding -nostdlib -lgcc

ARCH=arch
CORE=core
PROG=prog
LIB=lib
INC=inc
OBJ=obj
ELF=os.elf
TARGET=os.img
LINKER=$(ARCH)/memory.ld

SRC_ARCHS	:= $(wildcard $(ARCH)/*.s)
SRC_ARCHC   := $(wildcard $(ARCH)/*.c)
SRC_INC 	:= $(wildcard $(INC)/*.h)
SRC_CORE	:= $(wildcard $(CORE)/*.c)
SRC_PROG	:= $(wildcard $(PROG)/*.c)
SRC_LIB		:= $(wildcard $(LIB)/*.c)

OBJ_ARCHS	:= $(SRC_ARCHS:$(ARCH)/%.s=$(OBJ)/%.o)
OBJ_ARCHC   := $(SRC_ARCHC:$(ARCH)/%.c=$(OBJ)/%.o)
OBJ_CORE	:= $(SRC_CORE:$(CORE)/%.c=$(OBJ)/%.o)
OBJ_PROG	:= $(SRC_PROG:$(PROG)/%.c=$(OBJ)/%.o)
OBJ_LIB		:= $(SRC_LIB:$(LIB)/%.c=$(OBJ)/%.o)


all: $(TARGET)


run: $(TARGET)
	@echo "Starting qemu (Exit: Ctrl+A then X)"
	@qemu-system-arm -M versatilepb -m 128M -nographic -kernel $(TARGET)

clean:
	@echo "Clean"
	@-rm -rf $(OBJ)
	@-rm -f $(ELF)
	@-rm -f $(TARGET)


$(TARGET): $(OBJ) $(ELF) $(SRC_INC)
	@echo " CC\t" $(TARGET)
	@$(CC)-objcopy $(ELF) -O binary $(TARGET)

$(ELF): $(LINKER) $(OBJ_ARCHS) $(OBJ_ARCHC) $(OBJ_CORE) $(OBJ_LIB) $(OBJ_PROG)
	@echo " LINK"
	@$(CC)-gcc -T $(LINKER) $(OBJ_ARCHC) $(OBJ_ARCHS) $(OBJ_CORE) $(OBJ_LIB) $(OBJ_PROG)\
				-o $@ $(LDFLAGS)


$(OBJ_ARCHS): $(OBJ)/%.o : $(ARCH)/%.s
	@echo " CC\t" $<
	@$(CC)-gcc $(CFLAGS) $< -o $@

$(OBJ_ARCHC): $(OBJ)/%.o : $(ARCH)/%.c $(INC)
	@echo " CC\t" $<
	@$(CC)-gcc $(CFLAGS) -I$(INC) $< -o $@

$(OBJ_CORE): $(OBJ)/%.o : $(CORE)/%.c $(INC)
	@echo " CC\t" $<
	@$(CC)-gcc $(CFLAGS) -I$(INC) $< -o $@

$(OBJ_PROG): $(OBJ)/%.o : $(PROG)/%.c $(INC)
	@echo " CC\t" $<
	@$(CC)-gcc $(CFLAGS) -I$(INC) $< -o $@

$(OBJ_LIB): $(OBJ)/%.o : $(LIB)/%.c $(INC)
	@echo " CC\t" $<
	@$(CC)-gcc $(CFLAGS) -I$(INC) $< -o $@

$(OBJ):
	@echo " MK\t" $(OBJ)
	@mkdir $@
