/*
 * Copyright (C) 2008 Matthieu CASTET <castet.matthieu@free.fr>
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

#ifndef THREAD_PRIVATE_H
#define THREAD_PRIVATE_H
#include <linux/sched.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sched.h>
#include <sys/mman.h>

#ifndef PAGE_SIZE
// this need to be a constant, we can't use sysconf
#include <bits/uClibc_page.h>
#endif

#include "bionic_tls.h"
/*
 * This variable is 0 until a second thread is created.
 */
extern int __isthreaded;


void __thread_entry(int (*func)(void*), void *arg, void **tls);
static int tramp(void *_tls)
{
	void **tls = _tls;
	int (*fn)(void*) = tls[TLS_SLOT_MAX_WELL_KNOWN+1];
	void *arg = tls[TLS_SLOT_MAX_WELL_KNOWN+2];

	/* clean the hijacked tls ... */
	tls[TLS_SLOT_MAX_WELL_KNOWN+1] = 0;
	tls[TLS_SLOT_MAX_WELL_KNOWN+2] = 0;
	__thread_entry(fn, arg, tls);
	return 0;
}
static inline int  __pthread_clone(int (*fn)(void*), void **tls, int flags, void *arg)
{
	/* hijack tls to pass some data */
	tls[TLS_SLOT_MAX_WELL_KNOWN+1] = fn;
	tls[TLS_SLOT_MAX_WELL_KNOWN+2] = arg;
	return clone(tramp, tls /* stack */, flags, tls /* arg */);
}

static inline void _exit_thread(int  retCode)
{
	_exit(retCode);
}

static inline int tkill(int tid, int sig)
{
	return INLINE_SYSCALL(tkill, 2, tid, sig);
}

static inline int __rt_sigprocmask(int how, const sigset_t *set, sigset_t *oldset, size_t size)
{
	return sigprocmask(how, set, oldset);
}

static inline pid_t gettid(void)
{
	return INLINE_SYSCALL(gettid, 0);
}
#endif
