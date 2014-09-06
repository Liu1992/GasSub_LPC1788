/*
 * YAFFS: Yet Another Flash File System. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2011 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/* 
 * Example OS glue functions for running on a Linux/POSIX system.
 */

#include "ucos_ii.h"
#include "yaffscfg.h"
#include "yaffs_guts.h"
#include "yaffsfs.h"
#include "yaffs_trace.h"

#include <errno.h>

/*
 * yaffsfs_SetError() and yaffsfs_GetError()
 * Do whatever to set the system error.
 * yaffsfs_GetError() just fetches the last error.
 */

static int yaffsfs_lastError;

void yaffsfs_SetError(int err)
{
	//Do whatever to set error
	yaffsfs_lastError = err;
	errno = err;
}

int yaffsfs_GetLastError(void)
{
	return yaffsfs_lastError;
}

/*
 * yaffsfs_CheckMemRegion()
 * Check that access to an address is valid.
 * This can check memory is in bounds and is writable etc.
 *
 * Returns 0 if ok, negative if not.
 */
int yaffsfs_CheckMemRegion(const void *addr, size_t size, int write_request)
{
	if(!addr)
		return -1;
	return 0;
}

/*
 * yaffsfs_Lock()
 * yaffsfs_Unlock()
 * A single mechanism to lock and unlock yaffs. Hook up to a mutex or whatever.
 * Here are two examples, one using POSIX pthreads, the other doing nothing.
 */

static OS_EVENT *FsSem;

void yaffsfs_Lock(void)
{
    unsigned char err;
    OSSemPend(FsSem, 0, &err);
}

void yaffsfs_Unlock(void)
{
    OSSemPost(FsSem);
}

void yaffsfs_LockInit(void)
{
   FsSem = OSSemCreate(1);
}

/*
 * yaffsfs_CurrentTime() retrns a 32-bit timestamp.
 * 
 * Can return 0 if your system does not care about time.
 */
 
u32 yaffsfs_CurrentTime(void)
{
	return 0;
}


/*
 * yaffsfs_malloc()
 * yaffsfs_free()
 *
 * Functions to allocate and free memory.
 */
 
void *yaffsfs_malloc(size_t size)
{
	return malloc(size);
}

void yaffsfs_free(void *ptr)
{
	free(ptr);
}

void yaffsfs_OSInitialisation(void)
{
	yaffsfs_LockInit();
}

/*
 * yaffs_bug_fn()
 * Function to report a bug.
 */
 
void yaffs_bug_fn(const char *file_name, int line_no)
{
    printf("yaffs bug detected %s:%d\n",
		file_name, line_no);
}
