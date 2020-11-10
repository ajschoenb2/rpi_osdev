ARMGNU ?= aarch64-linux-gnu
OPT_LVL ?= -O3

INCLUDE_DIR = include
BUILD_DIR = build
SRC_DIR = src

CCFLAGS = -Wall -nostdlib -nostartfiles -fno-rtti -fno-exceptions -std=c++17 -ffreestanding -I$(INCLUDE_DIR) -g $(OPT_LVL) -mcpu=cortex-a53+nofp
CFLAGS = -Wall -nostdlib -nostdinc -nostartfiles -std=gnu99 -ffreestanding -I$(INCLUDE_DIR) -g $(OPT_LVL) -mcpu=cortex-a53+nofp
SFLAGS = -I$(INCLUDE_DIR)

all : kernel8.img

clean :
	rm -rf $(BUILD_DIR)

define DEFINE_COMPILE_RULES
$(BUILD_DIR)/%.o : $(1)%.cc
	@echo "compiling $$<"
	@mkdir -p $$(BUILD_DIR)
	$$(ARMGNU)-g++ $$(CCFLAGS) -MMD -c $$< -o $$@

$(BUILD_DIR)/%.o : $(1)%.c
	@echo "compiling $$<"
	@mkdir -p $$(BUILD_DIR)
	$$(ARMGNU)-gcc $$(CFLAGS) -MMD -c $$< -o $$@

$(BUILD_DIR)/%.o : $(1)%.S
	@echo "compiling $$<"
	@mkdir -p $$(BUILD_DIR)
	$$(ARMGNU)-gcc $$(SFLAGS) -MMD -c $$< -o $$@
endef

CCFILES = $(shell find . -name "*.cc")
CFILES = $(shell find . -name "*.c")
SFILES = $(shell find . -name "*.S")

SRCS = $(CCFILES) $(CFILES) $(SFILES)
OFILES = $(addprefix $(BUILD_DIR)/,$(notdir $(subst .cc,.o,$(CCFILES)) $(subst .c,.o,$(CFILES)) $(subst .S,.o,$(SFILES))))

$(foreach directory,$(sort $(dir $(SRCS))),$(eval $(call DEFINE_COMPILE_RULES,$(directory))))

-include $(BUILD_DIR)/*.d

kernel8.img : $(SRC_DIR)/link.ld Makefile $(OFILES)
	@echo "producing    kernel8.img"
	@mkdir -p build
	$(ARMGNU)-gcc -Wl,--build-id=none -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf -ffreestanding $(OPT_LVL) -nostdlib $(OFILES)
	# $(ARMGNU)-ld -T $(SRC_DIR)/link.ld -o $(BUILD_DIR)/kernel8.elf $(OFILES)
	# $(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary $(BUILD_DIR)/kernel8.bin
	# dd if=$(BUILD_DIR)/kernel8.bin of=$(BUILD_DIR)/kernel8.img bs=512 conv=sync > /dev/null 2>&1
	# qemu-img resize -f raw $(BUILD_DIR)/kernel8.img 16M
