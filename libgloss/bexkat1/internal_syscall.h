#ifndef _INTERNAL_SYSCALL_H
#define _INTERNAL_SYSCALL_H

static inline int
__internal_syscall(int n, int _r0)
{
  register int r0 asm("%0") = _r0;
  register int r12 asm("%12");
  
  asm volatile ("trap %1" : "+r"(r12) : "r"(syscall_id));
  return r12;
}

#endif
