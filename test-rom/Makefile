CC = riscv32-unknown-elf-gcc
LD = riscv32-unknown-elf-gcc
COMMONFLAGS = -Os -nostdlib -ffreestanding -nostartfiles -march=rv32im_zicsr -mabi=ilp32
CFLAGS = $(COMMONFLAGS) -mabi=ilp32
LDFLAGS = $(COMMONFLAGS) -mabi=ilp32

testrom.bin: build.c
	@ echo "Building objects"
	@ $(CC) -c $(CFLAGS) build.c -o obj/build.o
	@ $(CC) -c $(CFLAGS) boot.s -o obj/boot.o
	@ echo "Linking objects"
	@ $(LD) -T linker.ld $(LDFLAGS) obj/boot.o obj/build.o -o obj/testrom.elf
	@ llvm-objdump -d obj/testrom.elf > disassembly.txt
	@ llvm-objcopy -O binary obj/testrom.elf ../testrom.bin

clean:
	@ rm -r obj/*
