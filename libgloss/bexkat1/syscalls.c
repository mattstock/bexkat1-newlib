/*
 */

#include <sys/types.h>
#include <errno.h>

/*
 * unlink
 */
int
_DEFUN (_unlink, (name),
	const char *name)
{
  errno = ENOSYS;
  return -1;
}
