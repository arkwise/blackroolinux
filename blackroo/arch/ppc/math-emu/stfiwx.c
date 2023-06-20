/* $Id: stfiwx.c,v 1.1.1.1 2001/02/22 14:58:10 serg Exp $
 */

#include <linux/types.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

int
stfiwx(u32 *frS, void *ea)
{
#ifdef DEBUG
	printk("%s: %p %p\n", __FUNCTION__, frS, ea);
#endif

	if (copy_to_user(ea, &frS[1], sizeof(frS[1])))
		return -EFAULT;

	return 0;
}