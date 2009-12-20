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

#include <pthread.h>
#include <bits/uClibc_mutex.h>

/* we need to provide this function to uclibc
 * (taken from ./libc/misc/internals/__uClibc_main.c
weak_alias (__pthread_return_0, __pthread_mutex_init)
weak_alias (__pthread_return_0, __pthread_mutex_lock)
weak_alias (__pthread_return_0, __pthread_mutex_trylock)
weak_alias (__pthread_return_0, __pthread_mutex_unlock)
weak_alias (__pthread_return_void, _pthread_cleanup_push_defer)
weak_alias (__pthread_return_void, _pthread_cleanup_pop_restore)
*/

/* we use similar macro magic from linuxthread, but instead of
 * using a global array of function pointer, we use weak symbols
 * (the same logic behind __pthread_initialize_minimal)
 */
# define FORWARD2(name, rettype, decl, params, defaction, threadaction) \
extern rettype weak_function threadaction decl; \
rettype									      \
name decl								      \
{									      \
  if (threadaction == NULL)			      \
    defaction;								      \
									      \
  return threadaction params;			      \
}

# define FORWARD(name, decl, params, defretval, threadaction) \
  FORWARD2 (name, int, decl, params, return defretval, threadaction)

FORWARD (__pthread_mutex_init,
	 (pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr),
	 (mutex, mutexattr), 0, pthread_mutex_init)

FORWARD (__pthread_mutex_lock, (pthread_mutex_t *mutex), (mutex), 0, pthread_mutex_lock)
FORWARD (__pthread_mutex_trylock, (pthread_mutex_t *mutex), (mutex), 0, pthread_mutex_trylock)
FORWARD (__pthread_mutex_unlock, (pthread_mutex_t *mutex), (mutex), 0, pthread_mutex_unlock)
FORWARD2 (_pthread_cleanup_push_defer, void, (struct _pthread_cleanup_buffer * buffer, void (*routine)(void *), void * arg), (buffer, routine, arg), return, __pthread_cleanup_push)
FORWARD2 (_pthread_cleanup_pop_restore, void, (struct _pthread_cleanup_buffer * buffer, int execute), (buffer, execute), return, __pthread_cleanup_pop)

