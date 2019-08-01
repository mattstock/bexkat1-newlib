/*
 */

#include <sys/types.h>
#include <errno.h>

/*
 * unlink
 */
int
_unlink (const char *name)
{
  errno = ENOSYS;
  return -1;
}
