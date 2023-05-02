#ifndef _INTERNAL_SYSCALL_H
#define _INTERNAL_SYSCALL_H

#include <errno.h>

static inline long
__syscall_error (long r0)
{
  errno = -r0;
  return -1;
}

static inline long
__internal_syscall(int n, long _r0)
{
  register long r0 asm("%0") = _r0;
  register long r12 asm("%12");
  
  asm volatile ("trap %1" : "+r"(r12) : "I" (n));
  return r12;
}

#endif
