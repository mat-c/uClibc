/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef _SYS_ATOMICS_H
#define _SYS_ATOMICS_H

#include <sys/cdefs.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>

__BEGIN_DECLS

extern int __atomic_cmpxchg(int old, int _new, volatile int *ptr);
extern int __atomic_swap(int _new, volatile int *ptr);
extern int __atomic_dec(volatile int *ptr);
extern int __atomic_inc(volatile int *ptr);

/* this flag avoid to take a lock on linux kernel mm */
//#define FUTEX_PRIVATE_FLAG 128
/* XXX this is not true anymore since bionic support shared mutex */
#define FUTEX_PRIVATE_FLAG 0

#define FUTEX_WAIT (0|FUTEX_PRIVATE_FLAG)
#define FUTEX_WAKE (1|FUTEX_PRIVATE_FLAG)

static inline int __futex_wait(volatile void *ftx, int val, const struct timespec *timeout)
{
    int ret;
	ret=INLINE_SYSCALL(futex, 4, ftx,FUTEX_WAIT,val,timeout/*,NULL,0*/);
    return ret;
}

static inline int __futex_wake(volatile void *ftx, int count)
{
    int ret;
	ret=INLINE_SYSCALL(futex,4,ftx,FUTEX_WAKE,count,NULL/*,NULL,0*/);
    return ret;
}

__END_DECLS

#endif /* _SYS_ATOMICS_H */
