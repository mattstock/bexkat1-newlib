#include <machine/syscall.h>
#include "internal_syscall.h"

void
_exit(long exit_status)
{
  __internal_syscall (SYS_exit, exit_status);
  while (1);
}
