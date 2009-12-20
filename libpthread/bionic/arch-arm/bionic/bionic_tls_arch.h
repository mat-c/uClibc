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
/* see the implementation of __set_tls and pthread.c to understand this
 * code. Basically, the content of gs:[0] always is a pointer to the base
 * address of the tls region
 */
#ifndef BIONIC_TLS_ARCH_H
#define BIONIC_TLS_ARCH_H
static inline void*   __get_tls(void)
{
/* Linux kernel helpers for its TLS implementation */
/* For performance reasons, avoid calling the kernel helper
 * Note that HAVE_ARM_TLS_REGISTER is build-specific
 * (it must match your kernel configuration)
 */
#  ifdef HAVE_ARM_TLS_REGISTER
#    define __get_tls() \
    ({ register unsigned int __val asm("r0"); \
       asm ("mrc p15, 0, r0, c13, c0, 3" : "=r"(__val) ); \
       (volatile void*)__val; })
#  else /* !HAVE_ARM_TLS_REGISTER */
#    define __get_tls() ( *((volatile void **) 0xffff0ff0) )
#  endif
}
#endif
