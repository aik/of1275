all: build-all

build-all: of1275.bin

%.o: %.S
	cc -mbig -c -o $@ $<

%.o: %.c
	cc -mbig -c -fno-stack-protector -Wno-builtin-declaration-mismatch -o $@ $<

of1275.elf: entry.o main.o libc.o ci.o bootmem.o bswap.o bootblock.o elf32.o
	ld -nostdlib -e_start -Tl.lds -EB -o $@ $^

%.bin: %.elf
	objcopy -O binary -j .text -j .data $^ $@

clean:
	rm -f *.o *.bin *~
