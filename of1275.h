#include <stdarg.h>
#include <stdint.h>
#include "libelf.h"

//typedef unsigned char uint8_t;
//typedef unsigned short uint16_t;
//typedef unsigned long uint32_t;
//typedef unsigned long long uint64_t;
#define NULL (0)
#define PROM_ERROR (-1u)
typedef unsigned int __be32;
typedef unsigned char bool;
typedef unsigned int ihandle;
typedef unsigned int phandle;
#define false ((bool)0)
#define true ((bool)1)
typedef int size_t;
typedef void client(void);

/* globals */
extern void _prom_entry(void); /* OF CI entry point (i.e. this firmware ) */
typedef struct {
	phandle chosen;
	phandle rtas;
	ihandle istdout;
} of1275_t;
extern of1275_t of1275;

void do_boot(unsigned long addr, unsigned long r3, unsigned long r4);

/* libc */
int strlen(const char *s);
int strcmp(const char *s1, const char *s2);
void *memcpy(void *dest, const void *src, size_t n);
int memcmp(const void *ptr1, const void *ptr2, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *dest, int c, size_t size);

/* bswap */
uint32_t le32_to_cpu(uint32_t x);
uint64_t le64_to_cpu(uint64_t x);

/* Prom */
typedef unsigned int prom_arg_t;
int call_prom(const char *service, int nargs, int nret, ...);

/* CI wrappers */
void ci_init(void);
void ci_panic(const char *str);
phandle ci_finddevice(const char *path);
uint32_t ci_getprop(phandle ph, const char *propname, void *prop, int len);
ihandle ci_open(const char *path);
void ci_close(ihandle ih);
uint32_t ci_block_size(ihandle ih);
uint32_t ci_seek(ihandle ih, uint64_t offset);
uint32_t ci_read(ihandle ih, void *buf, int len);
uint32_t ci_write(ihandle ih, const void *buf, int len);
void ci_stdout(const char *buf);
void ci_stdoutn(const char *buf, int len);
void *ci_claim(void *virt, uint32_t size, uint32_t align);
uint32_t ci_release(void *virt, uint32_t size);

/* ELF */
static inline uint32_t load_elf(void *img, uint32_t *addr){ return 0; }

/* booting from blockdev */
void boot_block(void);

/* booting from -kernel */
void boot_from_memory(uint64_t initrd, uint64_t initrdsize);


/////////////////////////////////////////////////////
static inline uint16_t bswap_16 (uint16_t x)
{
        return __builtin_bswap16(x);
}

static inline uint32_t bswap_32 (uint32_t x)
{
        return __builtin_bswap32(x);
}

static inline uint64_t bswap_64 (uint64_t x)
{
        return __builtin_bswap64(x);
}

static inline void bswap_16p (uint16_t *x)
{
        *x = __builtin_bswap16(*x);
}

static inline void bswap_32p (uint32_t *x)
{
        *x = __builtin_bswap32(*x);
}

static inline void bswap_64p (uint64_t *x)
{
        *x = __builtin_bswap64(*x);
}

#define cpu_to_be16(x) (x)
#define cpu_to_be32(x) (x)
#define cpu_to_be64(x) (x)

#define be16_to_cpu(x) (x)
#define be32_to_cpu(x) (x)
#define be64_to_cpu(x) (x)

#define le16_to_cpu(x) bswap_16(x)
#define le32_to_cpu(x) bswap_32(x)
#define le64_to_cpu(x) bswap_64(x)

#define cpu_to_le16(x) bswap_16(x)
#define cpu_to_le32(x) bswap_32(x)
#define cpu_to_le64(x) bswap_64(x)
