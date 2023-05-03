#ifdef USING_SIM_SPECS

extern char _end[];
char *heap_ptr;

/*
 * sbrk -- changes heap size size. Get nbytes more
 *         RAM. We just increment a pointer in what's
 *         left of memory on the board.
 */
char *
_sbrk (nbytes)
     int nbytes;
{
  char *base;

  if (!heap_ptr)
    heap_ptr = (char *)&end;
  base = heap_ptr;
  heap_ptr += nbytes;

  return base;
}

#else

#include <machine/syscall.h>
#include <sys/types.h>
#include "internal_syscall.h"

void *
_sbrk (ptrdiff_t incr)
{
  static unsigned long heap_end;

  if (heap_end == 0)
    {
      long brk = __internal_syscall (SYS_brk, 0, 0, 0, 0, 0, 0, 0);
      if (brk == -1)
	return (void *)__syscall_error (-ENOMEM);
      heap_end = brk;
    }

  if (__internal_syscall (SYS_brk, 1, heap_end + incr, 0, 0, 0, 0, 0) != heap_end + incr)
    return (void *)__syscall_error (-ENOMEM);

  return (void *)(heap_end - incr);
}

#endif
