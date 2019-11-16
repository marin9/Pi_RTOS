CC=arm-none-eabi
MCPU=-mcpu=arm1176jzf-s
CFLAGS=-nostdlib -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-nostdlib -lgcc
HDFLAGS=-I$(CORE) -I$(CPU) -I$(DEV)

CPU=cpu
DEV=dev
CORE=core
PROG=prog
OBJ=obj

LINKER=$(DEV)/memory.ld
ELF=$(OBJ)/kernel.elf
TARGET=kernel.img

SRC_ASM		:= $(wildcard $(CPU)/*.S)
SRC_CPU		:= $(wildcard $(CPU)/*.c)
SRC_DEV		:= $(wildcard $(DEV)/*.c)
SRC_CORE	:= $(wildcard $(CORE)/*.c)
SRC_PROG	:= $(wildcard $(PROG)/*.c)
SRC_INC 	:= $(wildcard $(CPU)/*.h)
SRC_INC		+= $(wildcard $(DEV)/*.h)
SRC_INC		+= $(wildcard $(CORE)/*.h)

OBJ_ASM		:= $(SRC_ASM:$(CPU)/%.S=$(OBJ)/%.o)
OBJ_CPU		+= $(SRC_CPU:$(CPU)/%.c=$(OBJ)/%.o)
OBJ_DEV		:= $(SRC_DEV:$(DEV)/%.c=$(OBJ)/%.o)
OBJ_CORE	:= $(SRC_CORE:$(CORE)/%.c=$(OBJ)/%.o)
OBJ_PROG	:= $(SRC_PROG:$(PROG)/%.c=$(OBJ)/%.o)



$(TARGET): $(OBJ) $(ELF) $(SRC_INC)
	@echo " Create\t\t" $(TARGET)
	@$(CC)-objcopy $(ELF) -O binary $(TARGET)

$(ELF): $(LINKER) $(OBJ_ASM) $(OBJ_CPU) $(OBJ_DEV) \
		$(OBJ_CORE) $(OBJ_PROG)
	@echo " Linking\t" $(ELF)
	@$(CC)-gcc -T $(LINKER) $(OBJ_ASM) $(OBJ_CPU) \
			$(OBJ_DEV) $(OBJ_CORE) $(OBJ_PROG) \
			 -o $@ $(LDFLAGS)


$(OBJ_ASM): $(OBJ)/%.o : $(CPU)/%.S $(SRC_INC)
	@echo " Compile\t" $<
	@$(CC)-gcc $(MCPU) $(CFLAGS) -c $(HDFLAGS) $< -o $@

$(OBJ_CPU): $(OBJ)/%.o : $(CPU)/%.c $(SRC_INC)
	@echo " Compile\t" $<
	@$(CC)-gcc $(MCPU) $(CFLAGS) -c $(HDFLAGS) $< -o $@

$(OBJ_DEV): $(OBJ)/%.o : $(DEV)/%.c $(SRC_INC)
	@echo " Compile\t" $<
	@$(CC)-gcc $(MCPU) $(CFLAGS) -c $(HDFLAGS) $< -o $@

$(OBJ_CORE): $(OBJ)/%.o : $(CORE)/%.c $(SRC_INC)
	@echo " Compile\t" $<
	@$(CC)-gcc $(MCPU) $(CFLAGS) -c $(HDFLAGS) $< -o $@

$(OBJ_PROG): $(OBJ)/%.o : $(PROG)/%.c $(SRC_INC)
	@echo " Compile\t" $<
	@$(CC)-gcc $(MCPU) $(CFLAGS) -c $(HDFLAGS) $< -o $@

$(OBJ):
	@mkdir $@

clean:
	@echo "Clean"
	@-rm -rf $(OBJ)
	@-rm -f $(TARGET)
	