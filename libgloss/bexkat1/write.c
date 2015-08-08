#include <syscall.h>

static volatile unsigned int *serial0 = (unsigned int *)0x00800800;
static volatile unsigned int *serial1 = (unsigned int *)0x00800808;


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

int _write(int file, char *ptr, int len) {
  int todo;

  for (todo = 0; todo < len; todo++) {
    serial_putchar(file, *ptr++);
  }
  return len;
}
