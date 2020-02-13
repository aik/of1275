#define LOAD64(rn,name)                 \
        lis     rn,name##@highest;      \
        ori     rn,rn,name##@higher;    \
        rldicr  rn,rn,32,31;            \
        oris    rn,rn,name##@h;         \
        ori     rn,rn,name##@l

#define LOAD32(rn, name)                \
        lis     rn,name##@h;            \
        ori     rn,rn,name##@l

// load 32 bit constant in little endian order
#define LOAD32le(rn,name) \
        lis     rn,(((name>>8)&0x00FF)|((name<<8)&0xFF00));  \
        ori     rn,rn,(((name>>24)&0x00FF)|((name>>8)&0xFF00))

// load 16 bit constant in little endian order
#define LOAD16le(rn,name) \
        li      rn,(((name>>8)&0x00FF)|((name<<8)&0xFF00))

#define ENTRY(func_name)              \
        .text;                        \
        .align  2;                    \
        .globl  .func_name;           \
        .func_name:                   \
        .globl  func_name;            \
        func_name:

#define C_ENTRY(func_name)                      \
        .section        ".text";                \
        .align 2;                               \
        .globl func_name;                       \
        .section        ".opd","aw";            \
        .align 3;                               \
 func_name:                                     \
        .quad   .func_name,.TOC.@tocbase,0;     \
        .previous;                              \
        .size   func_name,24;                   \
        .type   .func_name,@function;           \
        .globl  .func_name;                     \
 .func_name:

#define ASM_ENTRY(fn)   \
        .globl  fn;     \
fn:

#if 0
/* Always big endian */
#define FIXUP_ENDIAN                                               \
        tdi   0,0,0x48;   /* Reverse endian of b . + 8          */ \
        b     $+44;       /* Skip trampoline if endian is good  */ \
        .long 0xa600607d; /* mfmsr r11                          */ \
        .long 0x01006b69; /* xori r11,r11,1                     */ \
        .long 0x00004039; /* li r10,0                           */ \
        .long 0x6401417d; /* mtmsrd r10,1                       */ \
        .long 0x05009f42; /* bcl 20,31,$+4                      */ \
        .long 0xa602487d; /* mflr r10                           */ \
        .long 0x14004a39; /* addi r10,r10,20                    */ \
        .long 0xa6035a7d; /* mtsrr0 r10                         */ \
        .long 0xa6037b7d; /* mtsrr1 r11                         */ \
        .long 0x2400004c  /* rfid                               */
#endif

#define KVMPPC_HCALL_BASE       0xf000
#define KVMPPC_H_RTAS           (KVMPPC_HCALL_BASE + 0x0)
#define KVMPPC_H_OF_CLIENT      (KVMPPC_HCALL_BASE + 0x5)

ENTRY(_start)
#	FIXUP_ENDIAN
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
