.PHONY: all clean kernel

all : Makefile kernel

clean :
	@$(MAKE) -C kernel --no-print-directory clean

kernel : Makefile
	@$(MAKE) -C kernel --no-print-directory
