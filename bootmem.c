#include "of1275.h"

void boot_from_memory(uint64_t initrd, uint64_t initrdsize)
{
	uint64_t kern[2];

	if (ci_getprop(of1275.chosen, "qemu,boot-kernel", kern, sizeof(kern)) !=
			sizeof(kern))
		return;

	do_boot(kern[0], initrd, initrdsize);
}
