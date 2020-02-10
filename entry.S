#define LOAD64(rn,name)                 \
        lis     rn,name##@highest;      \
        ori     rn,rn,name##@higher;    \
        rldicr  rn,rn,32,31;            \
        oris    rn,rn,name##@h;         \
        ori     rn,rn,name##@l

#define ENTRY(func_name)              \
        .text;                        \
        .align  2;                    \
        .globl  .func_name;           \
        .func_name:                   \
        .globl  func_name;            \
        func_name:

#define KVMPPC_HCALL_BASE       0xf000
#define KVMPPC_H_RTAS           (KVMPPC_HCALL_BASE + 0x0)
#define KVMPPC_H_OF_CLIENT      (KVMPPC_HCALL_BASE + 0x5)

ENTRY(_start)
	LOAD64(%r31, 0) /* Go 32bit mode */
	mtmsrd %r31,0
	LOAD64(2, __toc_start)
	b entry_c

ENTRY(_prom_entry)
	LOAD64(2, __toc_start)
	stdu    %r1,-112(%r1)
	std     %r31,104(%r1)
	mflr    %r31
	bl prom_entry
	nop
	mtlr    %r31
	ld      %r31,104(%r1)
	addi    %r1,%r1,112
	blr

ENTRY(ci_entry)
	mr	4,3
	lis	3,KVMPPC_H_OF_CLIENT@h
	ori	3,3,KVMPPC_H_OF_CLIENT@l
	sc	1
	blr

/* This is the actual RTAS blob copied to the OS at instantiate-rtas */
ENTRY(hv_rtas)
        mr      %r4,%r3
        lis     %r3,KVMPPC_H_RTAS@h
        ori     %r3,%r3,KVMPPC_H_RTAS@l
        sc	1
        blr
        .globl hv_rtas_size
hv_rtas_size:
        .long . - hv_rtas;
