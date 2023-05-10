/*****************************************************************************/

/*
 *	bios32.c -- PCI access code for embedded CO-MEM Lite PCI controller.
 *
 *	(C) Copyright 1999-2000, Greg Ungerer (gerg@moreton.com.au).
 *	(C) Copyright 2000, Lineo (www.lineo.com)
 */

/*****************************************************************************/

#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/types.h>
// DAVIDM #include <linux/tasks.h>
// DAVIDM #include <linux/bios32.h>
#include <linux/pci.h>
#include <linux/ptrace.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <asm/coldfire.h>
#include <asm/mcfsim.h>
#include <asm/irq.h>
#include <asm/anchor.h>

#ifdef CONFIG_eLIA
#include <asm/elia.h>
#endif

/*****************************************************************************/
#ifdef CONFIG_PCI
/*****************************************************************************/

/*
 *	Debug configuration defines. DEBUGRES sets debugging output for
 *	the resource allocation phase. DEBUGPCI traces on pcibios_ function
 *	calls, and DEBUGIO traces all accesses to devices on the PCI bus.
 */
/*#define	DEBUGRES	1*/
/*#define	DEBUGPCI	1*/
/*#define	DEBUGIO		1*/

/*****************************************************************************/

/*
 *	PCI markers for bus present and active slots.
 */
int		pci_bus_is_present = 0;
unsigned long	pci_slotmask = 0;

/*
 *	We may or may not need to swap the bytes of PCI bus tranfers.
 *	The endianess is re-roder automatically by the CO-MEM, but it
 *	will get the wrong byte order for a pure data stream.
 */
#define	pci_byteswap	0


/*
 *	Resource tracking. The CO-MEM part creates a virtual address
 *	space that all the PCI devices live in - it is not in any way
 *	directly mapped into the ColdFire address space. So we can
 *	really assign any resources we like to devices, as long as
 *	they do not clash with other PCI devices.
 */
unsigned int	pci_iobase = 0x100;		/* Arbitary start address */
unsigned int	pci_membase = 0x00010000;	/* Arbitary start address */

#define	PCI_MINIO	0x100			/* 256 byte minimum I/O */
#define	PCI_MINMEM	0x00010000		/* 64k minimum chunk */

/*
 *	The CO-MEM's shared memory segment is visible inside the PCI
 *	memory address space. We need to keep track of the address that
 *	this is mapped at, to setup the bus masters pointers.
 */
unsigned int	pci_shmemaddr;

/*****************************************************************************/

void 		pci_interrupt(int irq, void *id, struct pt_regs *fp);

/*****************************************************************************/

/*
 *	Some platforms have custom ways of reseting the PCI bus.
 */

void pci_resetbus(void)
{
#ifdef CONFIG_eLIA
	int	i;

#ifdef DEBUGPCI
	printk("pci_resetbus()\n");
#endif

	*((volatile unsigned short *) (MCF_MBAR+MCFSIM_PADDR)) |= eLIA_PCIRESET;
	for (i = 0; (i < 1000); i++) {
		*((volatile unsigned short *) (MCF_MBAR + MCFSIM_PADAT)) = 
			(ppdata | eLIA_PCIRESET);
	}


	*((volatile unsigned short *) (MCF_MBAR + MCFSIM_PADAT)) = ppdata;
#endif
}

/*****************************************************************************/

int pcibios_assignres(int slot)
{
	volatile unsigned long	*rp;
	volatile unsigned char	*ip;
	unsigned int		idsel, addr, val, align, i;
	int			bar;

#ifdef DEBUGPCI
	printk("pcibios_assignres(slot=%x)\n", slot);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	idsel = COMEM_DA_ADDR(0x1 << (slot + 16));

	/* Try to assign resource to each BAR */
	for (bar = 0; (bar < 6); bar++) {
		addr = COMEM_PCIBUS + PCI_BASE_ADDRESS_0 + (bar * 4);
		rp[LREG(COMEM_DAHBASE)] = COMEM_DA_CFGRD | idsel;
		val = rp[LREG(addr)];
#ifdef DEBUGRES
		printk("-----------------------------------"
			"-------------------------------------\n");
		printk("BAR[%d]: read=%08x ", bar, val);
#endif

		rp[LREG(COMEM_DAHBASE)] = COMEM_DA_CFGWR | idsel;
		rp[LREG(addr)] = 0xffffffff;

		rp[LREG(COMEM_DAHBASE)] = COMEM_DA_CFGRD | idsel;
		val = rp[LREG(addr)];
#ifdef DEBUGRES
		printk("write=%08x ", val);
#endif
		if (val == 0) {
#ifdef DEBUGRES
			printk("\n");
#endif
			continue;
		}

		/* Determine space required by BAR */
		/* FIXME: this should go backwords from 0x80000000... */
		for (i = 0; (i < 32); i++) {
			if ((0x1 << i) & (val & 0xfffffffc))
				break;
		}

#ifdef DEBUGRES
		printk("size=%08x(%d)\n", (0x1 << i), i);
#endif
		i = 0x1 << i;

		/* Assign a resource */
		if (val & PCI_BASE_ADDRESS_SPACE_IO) {
			if (i < PCI_MINIO)
				i = PCI_MINIO;
#ifdef DEBUGRES
			printk("BAR[%d]: IO size=%08x iobase=%08x\n",
				bar, i, pci_iobase);
#endif
			if (i > 0xffff) {
				/* Invalid size?? */
				val = 0 | PCI_BASE_ADDRESS_SPACE_IO;
#ifdef DEBUGRES
				printk("BAR[%d]: too big for IO??\n", bar);
#endif
			} else {
				/* Check for un-alignment */
				if ((align = pci_iobase % i))
					pci_iobase += (i - align);
				val = pci_iobase | PCI_BASE_ADDRESS_SPACE_IO;
				pci_iobase += i;
			}
		} else {
			if (i < PCI_MINMEM)
				i = PCI_MINMEM;
#ifdef DEBUGRES
			printk("BAR[%d]: MEMORY size=%08x membase=%08x\n",
				bar, i, pci_membase);
#endif
			/* Check for un-alignment */
			if ((align = pci_membase % i))
				pci_membase += (i - align);
			val = pci_membase | PCI_BASE_ADDRESS_SPACE_MEMORY;
			pci_membase += i;
		}

		/* Write resource back into BAR register */
		rp[LREG(COMEM_DAHBASE)] = COMEM_DA_CFGWR | idsel;
		rp[LREG(addr)] = val;
#ifdef DEBUGRES
		printk("BAR[%d]: assigned bar=%08x\n", bar, val);
#endif
	}

#ifdef DEBUGRES
	printk("-----------------------------------"
			"-------------------------------------\n");
#endif

	/* Assign IRQ if one is wanted... */
	ip = (volatile unsigned char *) (COMEM_BASE + COMEM_PCIBUS);
	rp[LREG(COMEM_DAHBASE)] = COMEM_DA_CFGRD | idsel;

	addr = (PCI_INTERRUPT_PIN & 0xfc) + (~PCI_INTERRUPT_PIN & 0x03);
	if (ip[addr]) {
		rp[LREG(COMEM_DAHBASE)] = COMEM_DA_CFGWR | idsel;
		addr = (PCI_INTERRUPT_LINE & 0xfc)+(~PCI_INTERRUPT_LINE & 0x03);
		ip[addr] = 25;
#ifdef DEBUGRES
		printk("IRQ LINE=25\n");
#endif
	}

	return(0);
}

/*****************************************************************************/

int pcibios_enable(int slot)
{
	volatile unsigned long	*rp;
	volatile unsigned short	*wp;
	unsigned int		idsel, addr;
	unsigned short		cmd;

#ifdef DEBUGPCI
	printk("pcibios_enbale(slot=%x)\n", slot);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	wp = (volatile unsigned short *) COMEM_BASE;
	idsel = COMEM_DA_ADDR(0x1 << (slot + 16));

	/* Get current command settings */
	addr = COMEM_PCIBUS + PCI_COMMAND;
	addr = (addr & ~0x3) + (~addr & 0x02);
	rp[LREG(COMEM_DAHBASE)] = COMEM_DA_CFGRD | idsel;
	cmd = wp[WREG(addr)];
	/*val = ((val & 0xff) << 8) | ((val >> 8) & 0xff);*/

	/* Enable I/O and memory accesses to this device */
	rp[LREG(COMEM_DAHBASE)] = COMEM_DA_CFGWR | idsel;
	cmd |= PCI_COMMAND_IO | PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER;
	wp[WREG(addr)] = cmd;

	return(0);
}

/*****************************************************************************/

unsigned long pcibios_init(unsigned long mem_start, unsigned long mem_end)
{
	volatile unsigned long	*rp;
	unsigned long		sel, id;
	int			slot;

#ifdef DEBUGPCI
	printk("pcibios_init()\n");
#endif

	pci_resetbus();

	/*
	 *	Do some sort of basic check to see if the CO-MEM part
	 *	is present... This works ok, but I think we really need
	 *	something better...
	 */
	rp = (volatile unsigned long *) COMEM_BASE;
	if ((rp[LREG(COMEM_LBUSCFG)] & 0xff) != 0x50) {
		printk("PCI: no PCI bus present\n");
		return(mem_start);
	}

#ifdef COMEM_BRIDGEDEV
	/*
	 *	Setup the PCI bridge device first. It needs resources too,
	 *	so that bus masters can get to its shared memory.
	 */
	slot = COMEM_BRIDGEDEV;
	sel = COMEM_DA_CFGRD | COMEM_DA_ADDR(0x1 << (slot + 16));
	rp[LREG(COMEM_DAHBASE)] = sel;
	rp[LREG(COMEM_PCIBUS)] = 0; /* Clear bus */
	id = rp[LREG(COMEM_PCIBUS)];
	if ((id == 0) || ((id & 0xffff0000) == (sel & 0xffff0000))) {
		printk("PCI: no PCI bus bridge present\n");
		return(mem_start);
	}

	printk("PCI: bridge device at slot=%d id=%08x\n", slot, (int) id);
	pci_slotmask |= 0x1 << slot;
	pci_shmemaddr = pci_membase;
	pcibios_assignres(slot);
	pcibios_enable(slot);
#endif

	pci_bus_is_present = 1;

	/*
	 *	Do a quick scan of the PCI bus and see what is here.
	 */
	for (slot = COMEM_MINDEV; (slot <= COMEM_MAXDEV); slot++) {
		sel = COMEM_DA_CFGRD | COMEM_DA_ADDR(0x1 << (slot + 16));
		rp[LREG(COMEM_DAHBASE)] = sel;
		rp[LREG(COMEM_PCIBUS)] = 0; /* Clear bus */
		id = rp[LREG(COMEM_PCIBUS)];
		if ((id != 0) && ((id & 0xffff0000) != (sel & 0xffff0000))) {
			printk("PCI: slot=%d id=%08x\n", slot, (int) id);
			pci_slotmask |= 0x1 << slot;
			pcibios_assignres(slot);
			pcibios_enable(slot);
		}
	}

	/* Get PCI irq for local vectoring */
	if (request_irq(COMEM_IRQ, pci_interrupt, 0, "PCI bridge", NULL)) {
		printk("PCI: failed to acquire interrupt %d\n", COMEM_IRQ);
	} else {
		mcf_autovector(COMEM_IRQ);
	}

	return(mem_start);
}

/*****************************************************************************/

unsigned long pcibios_fixup(unsigned long mem_start, unsigned long mem_end)
{
	return(mem_start);
}

/*****************************************************************************/

int pcibios_present(void)
{
	return(pci_bus_is_present);
}

/*****************************************************************************/

int pcibios_read_config_dword(unsigned char bus, unsigned char dev,
	unsigned char offset, unsigned int *val)
{
	volatile unsigned long	*rp;
	unsigned long		idsel, fnsel;

#ifdef DEBUGPCI
	printk("pcibios_read_config_dword(bus=%x,dev=%x,offset=%x,val=%x)\n",
		 bus, dev, offset, val);
#endif

	if (bus || ((pci_slotmask & (0x1 << PCI_SLOT(dev))) == 0)) {
		*val = 0xffffffff;
		return(PCIBIOS_SUCCESSFUL);
	}

	rp = (volatile unsigned long *) COMEM_BASE;
	idsel = COMEM_DA_CFGRD | COMEM_DA_ADDR(0x1 << ((dev >> 3) + 16));
	fnsel = (dev & 0x7) << 8;

	rp[LREG(COMEM_DAHBASE)] = idsel;
	*val = rp[LREG(COMEM_PCIBUS + (offset & 0xfc) + fnsel)];

#if 1
	/* If we get back what we wrote, then nothing there */
	/* This is pretty dodgy, but, hey, what else do we do?? */
	if (!offset && (*val == ((idsel & 0xfffff000) | (offset & 0x00000fff))))
		*val = 0xffffffff;
#endif

	return(PCIBIOS_SUCCESSFUL);
}

/*****************************************************************************/

int pcibios_read_config_word(unsigned char bus, unsigned char dev,
	unsigned char offset, unsigned short *val)
{
	volatile unsigned long	*rp;
	volatile unsigned short	*bp;
	unsigned long		idsel, fnsel;
	unsigned char		swapoffset;

#ifdef DEBUGPCI
	printk("pcibios_read_config_word(bus=%x,dev=%x,offset=%x)\n",
		bus, dev, offset);
#endif

	if (bus || ((pci_slotmask & (0x1 << PCI_SLOT(dev))) == 0)) {
		*val = 0xffff;
		return(PCIBIOS_SUCCESSFUL);
	}

	rp = (volatile unsigned long *) COMEM_BASE;
	bp = (volatile unsigned short *) COMEM_BASE;
	idsel = COMEM_DA_CFGRD | COMEM_DA_ADDR(0x1 << ((dev >> 3) + 16));
	fnsel = (dev & 0x7) << 8;
	swapoffset = (offset & 0xfc) + (~offset & 0x02);

	rp[LREG(COMEM_DAHBASE)] = idsel;
	*val = bp[WREG(COMEM_PCIBUS + swapoffset + fnsel)];

	return(PCIBIOS_SUCCESSFUL);
}

/*****************************************************************************/

int pcibios_read_config_byte(unsigned char bus, unsigned char dev,
	unsigned char offset, unsigned char *val)
{
	volatile unsigned long	*rp;
	volatile unsigned char	*bp;
	unsigned long		idsel, fnsel;
	unsigned char		swapoffset;

#ifdef DEBUGPCI
	printk("pcibios_read_config_byte(bus=%x,dev=%x,offset=%x)\n",
		bus, dev, offset);
#endif

	if (bus || ((pci_slotmask & (0x1 << PCI_SLOT(dev))) == 0)) {
		*val = 0xff;
		return(PCIBIOS_SUCCESSFUL);
	}

	rp = (volatile unsigned long *) COMEM_BASE;
	bp = (volatile unsigned char *) COMEM_BASE;
	idsel = COMEM_DA_CFGRD | COMEM_DA_ADDR(0x1 << ((dev >> 3) + 16));
	fnsel = (dev & 0x7) << 8;
	swapoffset = (offset & 0xfc) + (~offset & 0x03);

	rp[LREG(COMEM_DAHBASE)] = idsel;
	*val = bp[(COMEM_PCIBUS + swapoffset + fnsel)];

	return(PCIBIOS_SUCCESSFUL);
}

/*****************************************************************************/

int pcibios_write_config_dword(unsigned char bus, unsigned char dev,
	unsigned char offset, unsigned int val)
{
	volatile unsigned long	*rp;
	unsigned long		idsel, fnsel;

#ifdef DEBUGPCI
	printk("pcibios_write_config_dword(bus=%x,dev=%x,offset=%x,val=%x)\n",
		 bus, dev, offset, val);
#endif

	if (bus || ((pci_slotmask & (0x1 << PCI_SLOT(dev))) == 0))
		return(PCIBIOS_SUCCESSFUL);

	rp = (volatile unsigned long *) COMEM_BASE;
	idsel = COMEM_DA_CFGRD | COMEM_DA_ADDR(0x1 << ((dev >> 3) + 16));
	fnsel = (dev & 0x7) << 8;

	rp[LREG(COMEM_DAHBASE)] = idsel;
	rp[LREG(COMEM_PCIBUS + (offset & 0xfc) + fnsel)] = val;
	return(PCIBIOS_SUCCESSFUL);
}

/*****************************************************************************/

int pcibios_write_config_word(unsigned char bus, unsigned char dev,
	unsigned char offset, unsigned short val)
{

	volatile unsigned long	*rp;
	volatile unsigned short	*bp;
	unsigned long		idsel, fnsel;
	unsigned char		swapoffset;

#ifdef DEBUGPCI
	printk("pcibios_write_config_word(bus=%x,dev=%x,offset=%x,val=%x)\n",
		 bus, dev, offset, val);
#endif

	if (bus || ((pci_slotmask & (0x1 << PCI_SLOT(dev))) == 0))
		return(PCIBIOS_SUCCESSFUL);

	rp = (volatile unsigned long *) COMEM_BASE;
	bp = (volatile unsigned short *) COMEM_BASE;
	idsel = COMEM_DA_CFGRD | COMEM_DA_ADDR(0x1 << ((dev >> 3) + 16));
	fnsel = (dev & 0x7) << 8;
	swapoffset = (offset & 0xfc) + (~offset & 0x02);

	rp[LREG(COMEM_DAHBASE)] = idsel;
	bp[WREG(COMEM_PCIBUS + swapoffset + fnsel)] = val;
	return(PCIBIOS_SUCCESSFUL);
}

/*****************************************************************************/

int pcibios_write_config_byte(unsigned char bus, unsigned char dev,
	unsigned char offset, unsigned char val)
{
	volatile unsigned long	*rp;
	volatile unsigned char	*bp;
	unsigned long		idsel, fnsel;
	unsigned char		swapoffset;

#ifdef DEBUGPCI
	printk("pcibios_write_config_byte(bus=%x,dev=%x,offset=%x,val=%x)\n",
		 bus, dev, offset, val);
#endif

	if (bus || ((pci_slotmask & (0x1 << PCI_SLOT(dev))) == 0))
		return(PCIBIOS_SUCCESSFUL);

	rp = (volatile unsigned long *) COMEM_BASE;
	bp = (volatile unsigned char *) COMEM_BASE;
	idsel = COMEM_DA_CFGRD | COMEM_DA_ADDR(0x1 << ((dev >> 3) + 16));
	fnsel = (dev & 0x7) << 8;
	swapoffset = (offset & 0xfc) + (~offset & 0x03);

	rp[LREG(COMEM_DAHBASE)] = idsel;
	bp[(COMEM_PCIBUS + swapoffset + fnsel)] = val;
	return(PCIBIOS_SUCCESSFUL);
}

/*****************************************************************************/

int pcibios_find_device(unsigned short vendor, unsigned short devid,
	unsigned short index, unsigned char *bus, unsigned char *dev)
{
	unsigned int	vendev, val;
	unsigned char	devnr;

#ifdef DEBUGPCI
	printk("pcibios_find_device(vendor=%04x,devid=%04x,index=%d)\n",
		vendor, devid, index);
#endif

	if (vendor == 0xffff)
		return(PCIBIOS_BAD_VENDOR_ID);

	vendev = (devid << 16) | vendor;
	for (devnr = 0; (devnr < 32); devnr++) {
		pcibios_read_config_dword(0, devnr, PCI_VENDOR_ID, &val);
		if (vendev == val) {
			if (index-- == 0) {
				*bus = 0;
				*dev = devnr;
				return(PCIBIOS_SUCCESSFUL);
			}
		}
	}

	return(PCIBIOS_DEVICE_NOT_FOUND);
}

/*****************************************************************************/

int pcibios_find_class(unsigned int class, unsigned short index,
	unsigned char *bus, unsigned char *dev)
{
	unsigned int	val;
	unsigned char	devnr;

#ifdef DEBUGPCI
	printk("pcibios_find_class(class=%04x,index=%d)\n", class, index);
#endif

	/* FIXME: this ignores multi-function devices... */
	for (devnr = 0; (devnr < 128); devnr += 8) {
		pcibios_read_config_dword(0, devnr, PCI_CLASS_REVISION, &val);
		if ((val >> 8) == class) {
			if (index-- == 0) {
				*bus = 0;
				*dev = devnr;
				return(PCIBIOS_SUCCESSFUL);
			}
		}
	}

	return(PCIBIOS_DEVICE_NOT_FOUND);
}

/*****************************************************************************/

/*
 *	Local routines to interrcept the standard I/O and vector handling
 *	code. Don't include this 'till now - initialization code above needs
 *	access to the real code too.
 */
#include <asm/mcfpci.h>

/*****************************************************************************/

void pci_outb(unsigned char val, unsigned int addr)
{
	volatile unsigned long	*rp;
	volatile unsigned char	*bp;

#ifdef DEBUGIO
	printk("pci_outb(val=%02x,addr=%x)\n", val, addr);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	bp = (volatile unsigned char *) COMEM_BASE;
	rp[LREG(COMEM_DAHBASE)] = COMEM_DA_IOWR | COMEM_DA_ADDR(addr);
	addr = (addr & ~0x3) + (~addr & 0x03);
	bp[(COMEM_PCIBUS + COMEM_DA_OFFSET(addr))] = val;
}

/*****************************************************************************/

void pci_outw(unsigned short val, unsigned int addr)
{
	volatile unsigned long	*rp;
	volatile unsigned short	*sp;

#ifdef DEBUGIO
	printk("pci_outw(val=%04x,addr=%x)", val, addr);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	sp = (volatile unsigned short *) COMEM_BASE;
	rp[LREG(COMEM_DAHBASE)] = COMEM_DA_IOWR | COMEM_DA_ADDR(addr);
	addr = (addr & ~0x3) + (~addr & 0x02);
	if (pci_byteswap)
		val = ((val & 0xff) << 8) | ((val >> 8) & 0xff);
	sp[WREG(COMEM_PCIBUS + COMEM_DA_OFFSET(addr))] = val;
}

/*****************************************************************************/

void pci_outl(unsigned int val, unsigned int addr)
{
	volatile unsigned long	*rp;
	volatile unsigned int	*lp;

#ifdef DEBUGIO
	printk("pci_outl(val=%08x,addr=%x)\n", val, addr);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	lp = (volatile unsigned int *) COMEM_BASE;
	rp[LREG(COMEM_DAHBASE)] = COMEM_DA_IOWR | COMEM_DA_ADDR(addr);

	if (pci_byteswap)
		val = (val << 24) | ((val & 0x0000ff00) << 8) |
			((val & 0x00ff0000) >> 8) | (val >> 24);

	lp[LREG(COMEM_PCIBUS + COMEM_DA_OFFSET(addr))] = val;
}

/*****************************************************************************/

unsigned long	pci_blmask[] = {
	0x000000e0,
	0x000000d0,
	0x000000b0,
	0x00000070
};

unsigned char pci_inb(unsigned int addr)
{
	volatile unsigned long	*rp;
	volatile unsigned char	*bp;
	unsigned long		r;
	unsigned char		val;

#ifdef DEBUGIO
	printk("pci_inb(addr=%x)", addr);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	bp = (volatile unsigned char *) COMEM_BASE;

	r = COMEM_DA_IORD | COMEM_DA_ADDR(addr) | pci_blmask[(addr & 0x3)];
	rp[LREG(COMEM_DAHBASE)] = r;

	addr = (addr & ~0x3) + (~addr & 0x3);
	val = bp[(COMEM_PCIBUS + COMEM_DA_OFFSET(addr))];
	return(val);
}

/*****************************************************************************/

unsigned long	pci_bwmask[] = {
	0x000000c0,
	0x000000c0,
	0x00000030,
	0x00000030
};

unsigned short pci_inw(unsigned int addr)
{
	volatile unsigned long	*rp;
	volatile unsigned short	*sp;
	unsigned long		r;
	unsigned short		val;

#ifdef DEBUGIO
	printk("pci_inw(addr=%x)", addr);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	r = COMEM_DA_IORD | COMEM_DA_ADDR(addr) | pci_bwmask[(addr & 0x3)];
	rp[LREG(COMEM_DAHBASE)] = r;

	sp = (volatile unsigned short *) COMEM_BASE;
	addr = (addr & ~0x3) + (~addr & 0x02);
	val = sp[WREG(COMEM_PCIBUS + COMEM_DA_OFFSET(addr))];
	if (pci_byteswap)
		val = ((val & 0xff) << 8) | ((val >> 8) & 0xff);
#ifdef DEBUGIO
	printk("=%04x\n", val);
#endif
	return(val);
}

/*****************************************************************************/

unsigned int pci_inl(unsigned int addr)
{
	volatile unsigned long	*rp;
	volatile unsigned int	*lp;
	unsigned int		val;

#ifdef DEBUGIO
	printk("pci_inl(addr=%x)", addr);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	lp = (volatile unsigned int *) COMEM_BASE;
	rp[LREG(COMEM_DAHBASE)] = COMEM_DA_IORD | COMEM_DA_ADDR(addr);
	val = lp[LREG(COMEM_PCIBUS + COMEM_DA_OFFSET(addr))];

	if (pci_byteswap)
		val = (val << 24) | ((val & 0x0000ff00) << 8) |
			((val & 0x00ff0000) >> 8) | (val >> 24);

#ifdef DEBUGIO
	printk("=%08x\n", val);
#endif
	return(val);
}

/*****************************************************************************/

void pci_outsb(void *addr, void *buf, int len)
{
	volatile unsigned long	*rp;
	volatile unsigned char	*bp;
	unsigned char		*dp = (unsigned char *) buf;
	unsigned int		a = (unsigned int) addr;

#ifdef DEBUGIO
	printk("pci_outsb(addr=%x,buf=%x,len=%d)\n", (int)addr, (int)buf, len);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	rp[LREG(COMEM_DAHBASE)] = COMEM_DA_IOWR | COMEM_DA_ADDR(a);

	a = (a & ~0x3) + (~a & 0x03);
	bp = (volatile unsigned char *)
		(COMEM_BASE + COMEM_PCIBUS + COMEM_DA_OFFSET(a));

	while (len--)
		*bp = *dp++;
}

/*****************************************************************************/

void pci_outsw(void *addr, void *buf, int len)
{
	volatile unsigned long	*rp;
	volatile unsigned short	*wp;
	unsigned short		w, *dp = (unsigned short *) buf;
	unsigned int		a = (unsigned int) addr;

#ifdef DEBUGIO
	printk("pci_outsw(addr=%x,buf=%x,len=%d)\n", (int)addr, (int)buf, len);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	rp[LREG(COMEM_DAHBASE)] = COMEM_DA_IOWR | COMEM_DA_ADDR(a);

	a = (a & ~0x3) + (~a & 0x2);
	wp = (volatile unsigned short *)
		(COMEM_BASE + COMEM_PCIBUS + COMEM_DA_OFFSET(a));

	while (len--) {
		w = *dp++;
		if (pci_byteswap)
			w = ((w & 0xff) << 8) | ((w >> 8) & 0xff);
		*wp = w;
	}
}

/*****************************************************************************/

void pci_outsl(void *addr, void *buf, int len)
{
	volatile unsigned long	*rp;
	volatile unsigned long	*lp;
	unsigned long		l, *dp = (unsigned long *) buf;
	unsigned int		a = (unsigned int) addr;

#ifdef DEBUGIO
	printk("pci_outsl(addr=%x,buf=%x,len=%d)\n", (int)addr, (int)buf, len);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	rp[LREG(COMEM_DAHBASE)] = COMEM_DA_IOWR | COMEM_DA_ADDR(a);

	lp = (volatile unsigned long *)
		(COMEM_BASE + COMEM_PCIBUS + COMEM_DA_OFFSET(a));

	while (len--) {
		l = *dp++;
		if (pci_byteswap)
			l = (l << 24) | ((l & 0x0000ff00) << 8) |
				((l & 0x00ff0000) >> 8) | (l >> 24);
		*lp = l;
	}
}

/*****************************************************************************/

void pci_insb(void *addr, void *buf, int len)
{
	volatile unsigned long	*rp;
	volatile unsigned char	*bp;
	unsigned char		*dp = (unsigned char *) buf;
	unsigned int		a = (unsigned int) addr;

#ifdef DEBUGIO
	printk("pci_insb(addr=%x,buf=%x,len=%d)\n", (int)addr, (int)buf, len);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	rp[LREG(COMEM_DAHBASE)] = COMEM_DA_IORD | COMEM_DA_ADDR(a);

	a = (a & ~0x3) + (~a & 0x03);
	bp = (volatile unsigned char *)
		(COMEM_BASE + COMEM_PCIBUS + COMEM_DA_OFFSET(a));

	while (len--)
		*dp++ = *bp;
}

/*****************************************************************************/

void pci_insw(void *addr, void *buf, int len)
{
	volatile unsigned long	*rp;
	volatile unsigned short	*wp;
	unsigned short		w, *dp = (unsigned short *) buf;
	unsigned int		a = (unsigned int) addr;

#ifdef DEBUGIO
	printk("pci_insw(addr=%x,buf=%x,len=%d)\n", (int)addr, (int)buf, len);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	rp[LREG(COMEM_DAHBASE)] = COMEM_DA_IORD | COMEM_DA_ADDR(a);

	a = (a & ~0x3) + (~a & 0x2);
	wp = (volatile unsigned short *)
		(COMEM_BASE + COMEM_PCIBUS + COMEM_DA_OFFSET(a));

	while (len--) {
		w = *wp;
		if (pci_byteswap)
			w = ((w & 0xff) << 8) | ((w >> 8) & 0xff);
		*dp++ = w;
	}
}

/*****************************************************************************/

void pci_insl(void *addr, void *buf, int len)
{
	volatile unsigned long	*rp;
	volatile unsigned long	*lp;
	unsigned long		l, *dp = (unsigned long *) buf;
	unsigned int		a = (unsigned int) addr;

#ifdef DEBUGIO
	printk("pci_insl(addr=%x,buf=%x,len=%d)\n", (int)addr, (int)buf, len);
#endif

	rp = (volatile unsigned long *) COMEM_BASE;
	rp[LREG(COMEM_DAHBASE)] = COMEM_DA_IORD | COMEM_DA_ADDR(a);

	lp = (volatile unsigned long *)
		(COMEM_BASE + COMEM_PCIBUS + COMEM_DA_OFFSET(a));

	while (len--) {
		l = *lp;
		if (pci_byteswap)
			l = (l << 24) | ((l & 0x0000ff00) << 8) |
				((l & 0x00ff0000) >> 8) | (l >> 24);
		*dp++ = l;
	}
}

/*****************************************************************************/

struct pci_localirqlist {
	void		(*handler)(int, void *, struct pt_regs *);
	const char	*device;
	void		*dev_id;
};

struct pci_localirqlist	pci_irqlist[COMEM_MAXPCI];

/*****************************************************************************/

int pci_request_irq(unsigned int irq,
	void (*handler)(int, void *, struct pt_regs *),
	unsigned long flags, const char *device, void *dev_id)
{
	int	i;

#ifdef DEBUGIO
	printk("pci_request_irq(irq=%d,handler=%x,flags=%x,device=%s,"
		"dev_id=%x)\n", irq, (int) handler, (int) flags, device,
		(int) dev_id);
#endif

	/* Check if this interrupt handler is already lodged */
	for (i = 0; (i < COMEM_MAXPCI); i++) {
		if (pci_irqlist[i].handler == handler)
			return(0);
	}

	/* Find a free spot to put this handler */
	for (i = 0; (i < COMEM_MAXPCI); i++) {
		if (pci_irqlist[i].handler == 0) {
			pci_irqlist[i].handler = handler;
			pci_irqlist[i].device = device;
			pci_irqlist[i].dev_id = dev_id;
			return(0);
		}
	}

	/* Couldn't fit?? */
	return(1);
}

/*****************************************************************************/

void pci_free_irq(unsigned int irq, void *dev_id)
{
	int	i;

#ifdef DEBUGIO
	printk("pci_free_irq(irq=%d,dev_id=%x)\n", irq, (int) dev_id);
#endif

	if (dev_id == (void *) NULL)
		return;

	/* Check if this interrupt handler is lodged */
	for (i = 0; (i < COMEM_MAXPCI); i++) {
		if (pci_irqlist[i].dev_id == dev_id) {
			pci_irqlist[i].handler = NULL;
			pci_irqlist[i].device = NULL;
			pci_irqlist[i].dev_id = NULL;
			break;
		}
	}
}

/*****************************************************************************/

void pci_interrupt(int irq, void *id, struct pt_regs *fp)
{
	int	i;

#ifdef DEBUGIO
	printk("pci_interrupt(irq=%d,id=%x,fp=%x)\n", irq, (int) id, (int) fp);
#endif

	for (i = 0; (i < COMEM_MAXPCI); i++) {
		if (pci_irqlist[i].handler)
			(*pci_irqlist[i].handler)(irq,pci_irqlist[i].dev_id,fp);
	}
}

/*****************************************************************************/

/*
 *	The shared memory region is broken up into contiguous 512 byte
 *	regions for easy allocation... This is not an optimal solution
 *	but it makes allocation and freeing regions really easy.
 */

#define	PCI_MEMSLOTSIZE		512
#define	PCI_MEMSLOTS		(COMEM_SHMEMSIZE / PCI_MEMSLOTSIZE)

char	pci_shmemmap[PCI_MEMSLOTS];


void *pci_bmalloc(int size)
{
	int	i, j, nrslots;

#ifdef DEBUGIO
	printk("pci_bmalloc(size=%d)\n", size);
#endif

	if (size <= 0)
		return((void *) NULL);

	nrslots = (size - 1) / PCI_MEMSLOTSIZE;

	for (i = 0; (i < (PCI_MEMSLOTS-nrslots)); i++) {
		if (pci_shmemmap[i] == 0) {
			for (j = i+1; (j < (i+nrslots)); j++) {
				if (pci_shmemmap[j])
					goto restart;
			}

			for (j = i; (j <= i+nrslots); j++)
				pci_shmemmap[j] = 1;
			break;
		}
restart:
	}

	return((void *) (COMEM_BASE + COMEM_SHMEM + (i * PCI_MEMSLOTSIZE)));
}

/*****************************************************************************/

void pci_bmfree(void *mp, int size)
{
	int	i, j, nrslots;

#ifdef DEBUGIO
	printk("pci_bmfree(mp=%x,size=%d)\n", (int) mp, size);
#endif

	nrslots = size / PCI_MEMSLOTSIZE;
	i = (((unsigned long) mp) - (COMEM_BASE + COMEM_SHMEM)) /
		PCI_MEMSLOTSIZE;

	for (j = i; (j < (i+nrslots)); j++)
		pci_shmemmap[j] = 0;
}

/*****************************************************************************/

unsigned long pci_virt_to_bus(volatile void *address)
{
	unsigned long	l;

#ifdef DEBUGIO
	printk("pci_virt_to_bus(address=%x)", (int) address);
#endif

	l = ((unsigned long) address) - COMEM_BASE;
#ifdef DEBUGIO
	printk("=%x\n", (int) (l+pci_shmemaddr));
#endif
	return(l + pci_shmemaddr);
}

/*****************************************************************************/

void *pci_bus_to_virt(unsigned long address)
{
	unsigned long	l;

#ifdef DEBUGIO
	printk("pci_bus_to_virt(address=%x)", (int) address);
#endif

	l = address - pci_shmemaddr;
#ifdef DEBUGIO
	printk("=%x\n", (int) (address + COMEM_BASE));
#endif
	return((void *) (address + COMEM_BASE));
}

/*****************************************************************************/

void pci_bmcpyto(void *dst, void *src, int len)
{
	unsigned long	*dp, *sp, val;
	unsigned char	*dcp, *scp;
	int		i, j;

#ifdef DEBUGIO
	printk("pci_bmcpyto(dst=%x,src=%x,len=%d)\n", (int)dst, (int)src, len);
#endif

	dp = (unsigned long *) dst;
	sp = (unsigned long *) src;
	i = len >> 2;

#if 0
	printk("DATA:");
	scp = (unsigned char *) sp;
	for (i = 0; (i < len); i++) {
		if ((i % 16) == 0) printk("\n%04x: ", i);
		printk("%02x ", *scp++);
	}
	printk("\n");
#endif

	for (j = 0; (i >= 0); i--, j++) {
		val = *sp++;
		val = (val << 24) | ((val & 0x0000ff00) << 8) |
			((val & 0x00ff0000) >> 8) | (val >> 24);
		*dp++ = val;
	}

	if (len & 0x3) {
		dcp = (unsigned char *) dp;
		scp = ((unsigned char *) sp) + 3;
		for (i = 0; (i < (len & 0x3)); i++)
			*dcp++ = *scp--;
	}
}

/*****************************************************************************/

void pci_bmcpyfrom(void *dst, void *src, int len)
{
	unsigned long	*dp, *sp, val;
	unsigned char	*dcp, *scp;
	int		i;

#ifdef DEBUGIO
	printk("pci_bmcpyfrom(dst=%x,src=%x,len=%d)\n",(int)dst,(int)src,len);
#endif

	dp = (unsigned long *) dst;
	sp = (unsigned long *) src;
	i = len >> 2;

	for (; (i >= 0); i--) {
		val = *sp++;
		val = (val << 24) | ((val & 0x0000ff00) << 8) |
			((val & 0x00ff0000) >> 8) | (val >> 24);
		*dp++ = val;
	}

	if (len & 0x3) {
		dcp = ((unsigned char *) dp) + 3;
		scp = (unsigned char *) sp;
		for (i = 0; (i < (len & 0x3)); i++)
			*dcp++ = *scp--;
	}

#if 0
	printk("DATA:");
	dcp = (unsigned char *) dst;
	for (i = 0; (i < len); i++) {
		if ((i % 16) == 0) printk("\n%04x: ", i);
		printk("%02x ", *dcp++);
	}
	printk("\n");
#endif
}

/*****************************************************************************/
#endif	/* CONFIG_PCI */
