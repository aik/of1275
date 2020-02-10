#include "of1275.h"

struct prom_args {
        __be32 service;
        __be32 nargs;
        __be32 nret;
        __be32 args[10];
};

#define CELL(x) (((__be32)(unsigned long)(x)))
#define ADDR(x) CELL(x)

extern __be32 ci_entry(__be32 params);

extern unsigned long hv_rtas(unsigned long params);
extern unsigned int hv_rtas_size;

bool prom_handle(struct prom_args *pargs)
{
	void *rtasbase;
	uint32_t rtassize = 0;

	if (strcmp("call-method", (void *)(unsigned long) pargs->service))
		return false;

	if (strcmp("instantiate-rtas", (void *)(unsigned long) pargs->args[0]))
		return false;

	ci_getprop(of1275.rtas, "rtas-size", &rtassize, sizeof(rtassize));
	if (rtassize < hv_rtas_size)
		return false;

	rtasbase = (void *)(unsigned long) pargs->args[2];

	memcpy(rtasbase, hv_rtas, hv_rtas_size);
	pargs->args[pargs->nargs] = 0;
	pargs->args[pargs->nargs + 1] = pargs->args[2];

	return true;
}

void prom_entry(__be32 args)
{
	if (!prom_handle((void *)(unsigned long) args))
		ci_entry(args);
}

int call_prom(const char *service, int nargs, int nret, ...)
{
        int i;
        struct prom_args args;
        va_list list;

        args.service = cpu_to_be32(ADDR(service));
        args.nargs = cpu_to_be32(nargs);
        args.nret = cpu_to_be32(nret);

        va_start(list, nret);
        for (i = 0; i < nargs; i++)
                args.args[i] = cpu_to_be32(va_arg(list, prom_arg_t));
        va_end(list);

        for (i = 0; i < nret; i++)
                args.args[nargs+i] = 0;

        if (ci_entry(CELL(&args)) < 0)
                return PROM_ERROR;

        return (nret > 0) ? be32_to_cpu(args.args[nargs]) : 0;
}

void ci_init(void)
{
	of1275.chosen = ci_finddevice("/chosen");
	of1275.rtas = ci_finddevice("/rtas");

	if (!of1275.chosen)
		ci_panic("No /chosen");
	if (!of1275.rtas)
		ci_panic("No /rtas");

	ci_getprop(of1275.chosen, "stdout", &of1275.istdout,
			sizeof(of1275.istdout));
}

void ci_panic(const char *str)
{
	call_prom("exit", 0, 0);
}

phandle ci_finddevice(const char *path)
{
	return call_prom("finddevice", 1, 1, path);
}

uint32_t ci_getprop(phandle ph, const char *propname, void *prop, int len)
{
	return call_prom("getprop", 4, 1, ph, propname, prop, len);
}

ihandle ci_open(const char *path)
{
	return call_prom("open", 1, 1, path);
}

void ci_close(ihandle ih)
{
	call_prom("close", 1, 0, ih);
}

uint32_t ci_block_size(ihandle ih)
{
	return 512;
}

uint32_t ci_seek(ihandle ih, uint64_t offset)
{
	return call_prom("seek", 3, 1, ih, offset >> 32, offset & 0xFFFFFFFF);
}

uint32_t ci_read(ihandle ih, void *buf, int len)
{
	return call_prom("read", 3, 1, ih, buf, len);
}

uint32_t ci_write(ihandle ih, const void *buf, int len)
{
	return call_prom("write", 3, 1, ih, buf, len);
}

void ci_stdoutn(const char *buf, int len)
{
	ci_write(of1275.istdout, buf, len);
}

void ci_stdout(const char *buf)
{
	ci_stdoutn(buf, strlen(buf));
}

void *ci_claim(void *virt, uint32_t size, uint32_t align)
{
	uint32_t ret = call_prom("claim", 3, 1, ADDR(virt), size, align);

	return (void *) (unsigned long) ret;
}

uint32_t ci_release(void *virt, uint32_t size)
{
	return call_prom("release", 2, 1, ADDR(virt), size);
}
