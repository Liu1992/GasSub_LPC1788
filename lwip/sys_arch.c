/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *         Simon Goldschmidt
 *
 */

#include <stdlib.h>
#include <stdio.h> /* sprintf() for task names */
#include "queue.h"

#include <time.h>

#include <lwip/opt.h>
#include <lwip/arch.h>
#include <lwip/stats.h>
#include <lwip/debug.h>
#include <lwip/sys.h>
#include <ucos_ii.h>

#define NMBOX		64
#define MBOXSLOTS	32

struct sys_mbox_entry {
    int freed;
    int head, nextq;
    void *msg[MBOXSLOTS];
    sys_sem_t queued_msg;
    sys_sem_t free_msg;
    LIST_ENTRY(sys_mbox_entry) link;
};
static struct sys_mbox_entry mboxes[NMBOX];
static LIST_HEAD(mbox_list, sys_mbox_entry) mbox_free;

static u32_t sys_get_ms()
{
    return OSTimeGet();
}

u32_t sys_jiffies()
{
  return (u32_t)sys_get_ms();
}

u32_t sys_now()
{
  return (u32_t)sys_get_ms();
}

u32_t sys_arch_protect()
{
    OSSchedLock();
    return 0;
}
void sys_arch_unprotect(u32_t pval)
{
    (void)pval;
    OSSchedUnlock();
}

void sys_init()
{
    int i;
    for (i = 0; i < NMBOX; i++) {
        mboxes[i].freed = 1;
        LIST_INSERT_HEAD(&mbox_free, &mboxes[i], link);
    }
}

#if !NO_SYS

struct threadlist *lwip_win32_threads = NULL;

void do_sleep(int ms)
{
    OSTimeDlyHMSM(0, 0, 0, ms);
}

err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
    sem->sem = OSSemCreate(count);
    if (!sem->sem)
        return ERR_MEM;
    sem->valid = 1;
    return ERR_OK;
}

void sys_sem_free(sys_sem_t *sem)
{
    INT8U err;
    OSSemDel(sem->sem, OS_DEL_ALWAYS, &err);
    sem->sem = NULL;
}

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    INT8U err;
    u32_t s, e;
    s = sys_get_ms();
    OSSemPend(sem->sem, timeout, &err);
    e = sys_get_ms();
    if (err == OS_ERR_TIMEOUT)
        return SYS_ARCH_TIMEOUT;
    return e - s;
}

u32_t sys_arch_sem_trywait(sys_sem_t *sem)
{
    INT8U err;
    OS_SEM_DATA sd;
    
    sys_arch_protect();
    OSSemQuery(sem->sem, &sd);
    if (sd.OSCnt > 0)
    {
        OSSemSet(sem->sem, sd.OSCnt - 1, &err);
        // err ?
        sys_arch_unprotect(0);
        return 0;
    }
    else
    {
        sys_arch_unprotect(0);
        return SYS_ARCH_TIMEOUT;
    }
}

void sys_sem_signal(sys_sem_t *sem)
{
    OSSemPost(sem->sem);
}

sys_thread_t sys_thread_new(const char *name, lwip_thread_fn function, void *arg, int stacksize, int prio)
{
    OS_STK *pstack;
    
    (void)prio;
    if (stacksize <= 0)
        stacksize = 4096;
    
    pstack = malloc(stacksize * sizeof(OS_STK));
    OSTaskCreateExt((void (*)(void *))function,            /* Create the start task                                */
                    (void          *)0,
                    (OS_STK        *)&pstack[stacksize - 1],
                    (INT8U          )prio,
                    (INT16U         )prio,
                    (OS_STK        *)&pstack[0],
                    (INT32U         )stacksize,
                    (void          *)0,
                    (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    return 0;
}

err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
    struct sys_mbox_entry *mbe;

    sys_arch_protect();
    mbe = LIST_FIRST(&mbox_free);
    if (!mbe) {
        // cprintf("lwip: sys_mbox_new: out of mailboxes\n");
        sys_arch_unprotect(0);
        return ERR_MEM;
    }
    LIST_REMOVE(mbe, link);
    // assert(mbe->freed);
    mbe->freed = 0;
    mbox->mbox = mbe - &mboxes[0];
    mbox->valid = 1;
    mbe->head = -1;
    mbe->nextq = 0;
    sys_arch_unprotect(0);
    
    if (sys_sem_new(&mbe->queued_msg, 0) != ERR_OK ||
        sys_sem_new(&mbe->free_msg, MBOXSLOTS) != ERR_OK)
    {
        sys_mbox_free(mbox);
        return ERR_MEM;
    }
    return ERR_OK;
}

void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
    int slot;
    // assert(!mboxes[mbox].freed);
    sys_arch_sem_wait(&mboxes[mbox->mbox].free_msg, 0);
    
    sys_arch_protect();
    slot = mboxes[mbox->mbox].nextq;
    mboxes[mbox->mbox].nextq = (slot + 1) % MBOXSLOTS;
    mboxes[mbox->mbox].msg[slot] = msg;
    if (mboxes[mbox->mbox].head == -1)
        mboxes[mbox->mbox].head = slot;
    sys_arch_unprotect(0);

    sys_sem_signal(&mboxes[mbox->mbox].queued_msg);
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{
    int slot;
    
    // try to consume a sem
    if (sys_arch_sem_trywait(&mboxes[mbox->mbox].free_msg) == SYS_ARCH_TIMEOUT)
        return ERR_MEM;

    sys_arch_protect();
    slot = mboxes[mbox->mbox].nextq;
    mboxes[mbox->mbox].nextq = (slot + 1) % MBOXSLOTS;
    mboxes[mbox->mbox].msg[slot] = msg;
    if (mboxes[mbox->mbox].head == -1)
        mboxes[mbox->mbox].head = slot;
    sys_arch_unprotect(0);
    
    sys_sem_signal(&mboxes[mbox->mbox].queued_msg);
    
    return ERR_OK;
}

void
sys_mbox_free(sys_mbox_t *mbox)
{
    // assert(!mboxes[*mbox].freed);
    sys_arch_protect();
    sys_sem_free(&mboxes[mbox->mbox].queued_msg);
    sys_sem_free(&mboxes[mbox->mbox].free_msg);
    LIST_INSERT_HEAD(&mbox_free, &mboxes[mbox->mbox], link);
    mboxes[mbox->mbox].freed = 1;
    sys_arch_unprotect(0);
}

u32_t
sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t tm_msec)
{
    int slot;
    // assert(!mboxes[mbox].freed);

    u32_t waited = sys_arch_sem_wait(&mboxes[mbox->mbox].queued_msg, tm_msec);
    if (waited == SYS_ARCH_TIMEOUT)
        return waited;

    sys_arch_protect();
    slot = mboxes[mbox->mbox].head;
    if (slot == -1)
    {
        // panic("lwip: sys_arch_mbox_fetch: no message");
    }
    if (msg)
        *msg = mboxes[mbox->mbox].msg[slot];

    mboxes[mbox->mbox].head = (slot + 1) % MBOXSLOTS;
    if (mboxes[mbox->mbox].head == mboxes[mbox->mbox].nextq)
	mboxes[mbox->mbox].head = -1;
    sys_arch_unprotect(0);

    sys_sem_signal(&mboxes[mbox->mbox].free_msg);
    return waited;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *q, void **msg)
{
    return sys_arch_mbox_fetch(q, msg, 1);
}

#endif /* !NO_SYS */
