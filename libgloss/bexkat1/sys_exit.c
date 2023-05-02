#include <machine/syscall.h>
#include "internal_syscall.h"

void
_exit(int exit_status)
{
  _internal_syscall (SYS_exit, exit_status);
  while (1);
}
