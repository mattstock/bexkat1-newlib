#include <syscall.h>

static volatile unsigned int *serial0 = (unsigned int *)0x00800800;
static volatile unsigned int *serial1 = (unsigned int *)0x00800808;

char serial_getchar(unsigned port) {
  unsigned result;
  volatile unsigned *p;

  switch (port) {
  case 3:
    p = serial1;
    break;
  default:
    p = serial0;
  }

  result  = p[0];
  while ((result & 0x8000) == 0)
    result = p[0];
  return (char)(result & 0xff); 
}

int _read(int file, char *ptr, int len) {
  int idx = 0;

  while (idx < len) {
    ptr[idx++] = serial_getchar(file);
  }
  return len;
}
