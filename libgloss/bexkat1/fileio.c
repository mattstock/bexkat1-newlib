#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "glue.h"

int
_DEFUN (_stat, (path, buf),
       const char *path _AND
       struct stat *buf)
{
  errno = EIO;
  return (-1);
}

int
_DEFUN (_fstat, (fd, buf),
       int fd _AND
       struct stat *buf)
{
  buf->st_mode = S_IFCHR;	/* Always pretend to be a tty */
  buf->st_blksize = 0;

  return (0);
}

int
_DEFUN (_close, (file),
	int file)
{
  errno = EIO;
  return -1;
}

int
_DEFUN (_access, (path, mode),
	const char *path _AND
	int mode)
{
  errno = EIO;
  return -1;
}

int
_DEFUN (_mkdir, (path, mode),
	const char *path _AND
	mode_t mode)
{
  errno = EIO;
  return -1;
}

int
_DEFUN (_open, (path, flags, mode),
	const char *path _AND
	int flags _AND
	int mode)
{
  errno = EIO;
  return -1;
}

off_t
_DEFUN (_lseek, (fd,  offset, whence),
       int fd _AND
       off_t offset _AND
       int whence)
{
  errno = ESPIPE;
  return ((off_t)-1);
}

static volatile unsigned int *serial0 = (unsigned int *)0x00800800;
static volatile unsigned int *serial1 = (unsigned int *)0x00800808;

static char inbyte() {
  unsigned result;
  volatile unsigned *p;

  p = serial0;

  result  = p[0];
  while ((result & 0x8000) == 0)
    result = p[0];
  return (char)(result & 0xff); 
}

static void serial_putchar(int port, char c) {
  volatile unsigned *p;

  switch (port) {
  case 3:
    p = serial1;
    break;
  default:
    p = serial0;
  }

  while (!(p[0] & 0x2000));
  p[0] = (unsigned)c;
}

int
_DEFUN (_read, (file, buf, nbytes),
	int file _AND
	char *buf _AND
	int nbytes)
{
  int i = 0;

  for (i = 0; i < nbytes; i++) {
    *(buf + i) = inbyte();
    if ((*(buf + i) == '\n') || (*(buf + i) == '\r')) {
      i++;
      break;
    }
  }
  return (i);
}

int
_DEFUN (_write, (file, ptr, len),
	int file _AND
	char *ptr _AND
	int len)
{
  int todo;

  for (todo = 0; todo < len; todo++) {
    serial_putchar(file, *ptr++);
  }
  return len;
}

