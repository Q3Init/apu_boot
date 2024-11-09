#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "Platform_Types.h"
/**
 * device_init_null() - Void registration routine of a device driver
 *
 * This routine implements a void registration routine of a device
 * driver. The registration routine of a particular driver is aliased
 * to this empty function in case the driver is not compiled into
 * U-Boot.
 */
static void device_null(void)
{
}

/**
 * DEVICE_INITFUNC() - Forward declare of driver registration routine
 * @name:	Name of the real driver registration routine.
 *
 * This macro expands onto forward declaration of a driver registration
 * routine, which is then used below in device_initialize() function.
 * The declaration is made weak and aliases to device_null() so in case
 * the driver is not compiled in, the function is still declared and can
 * be used, but aliases to device_null() and thus is optimized away.
 */
#define DEVICE_INITFUNC(name) \
	void name(void)           \
		__attribute__((weak, alias("device_null")));

#endif
