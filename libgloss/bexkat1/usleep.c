/* Copied from libc/posix/usleep.c, removed the check for HAVE_NANOSLEEP */

/* Written 2002 by Jeff Johnston */

#include <errno.h>
#include <time.h>
#include <unistd.h>

char usleep_value;

int usleep(useconds_t useconds)
{
  int i;
  for (i=0; i < useconds*20; i++)
    usleep_value++;
}
