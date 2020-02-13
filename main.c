#include "of1275.h"

of1275_t of1275;

void do_boot(unsigned long addr, unsigned long _r3, unsigned long _r4)
{
	register unsigned long r3 __asm__("r3") = _r3;
	register unsigned long r4 __asm__("r4") = _r4;
	register unsigned long r5 __asm__("r5") = (unsigned long) _prom_entry;

	((client *)(uint64_t)addr)();
}

static char *hello = "*** Bootloader ***\n";

void entry_c(void)
{
	register unsigned long r3 __asm__("r3");
	register unsigned long r4 __asm__("r4");
	register unsigned long r5 __asm__("r5");
	uint64_t initrd = r3, initrdsize = r4;

	ci_init();
	ci_stdout(hello);
	boot_from_memory(initrd, initrdsize);
	boot_block();
}
