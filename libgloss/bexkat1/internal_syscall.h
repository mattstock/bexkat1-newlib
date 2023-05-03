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
__internal_syscall(long n, int argc, long _a0, long _a1, long _a2, long _a3, long _a4, long _a5)
{

  register long r12 asm("%12");

  if (argc == 0) {
    asm volatile ("trap %1" : "+r"(r12) : "I"(n));
    return r12;
  }
  register long r0 asm("%0") = _a0;
  if (argc == 1) {
    asm volatile ("trap %1" : "+r"(r12) : "I"(n), "r"(r0));
    return r12;
  }
  register long r1 asm("%1") = _a1;
  if (argc == 1) {
    asm volatile ("trap %1" : "+r"(r12) : "I"(n), "r"(r0), "r"(r1));
    return r12;
  }
  register long r2 asm("%2") = _a2;
  if (argc == 1) {
    asm volatile ("trap %1" : "+r"(r12) : "I"(n), "r"(r0), "r"(r1), "r"(r2));
    return r12;
  }
  register long r3 asm("%3") = _a3;
  if (argc == 1) {
    asm volatile ("trap %1" : "+r"(r12) : "I"(n), "r"(r0), "r"(r1), "r"(r2),
		  "r"(r3));
    return r12;
  }
  register long r4 asm("%4") = _a4;
  if (argc == 1) {
    asm volatile ("trap %1" : "+r"(r12) : "I"(n), "r"(r0), "r"(r1), "r"(r2),
		  "r"(r3), "r"(r4));
    return r12;
  }
  register long r5 asm("%5") = _a5;
  asm volatile ("trap %1" : "+r"(r12) : "I"(n), "r"(r0), "r"(r1), "r"(r2),
		"r"(r3), "r"(r4), "r"(r5));
  return r12;
}

static inline long
syscall_errno(long n, int argc, long _a0, long _a1, long _a2, long _a3, long _a4, long _a5)
{
  long a0 = __internal_syscall (n, argc, _a0, _a1, _a2, _a3, _a4, _a5);

  if (a0 < 0)
    return __syscall_error (a0);
  else
    return a0;
}

#endif
