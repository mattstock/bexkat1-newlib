#include <syscall.h>

static volatile unsigned int *serial0 = (unsigned int *)0x00800800;
static volatile unsigned int *serial1 = (unsigned int *)0x00800808;

char inbyte() {
  unsigned result;
  volatile unsigned *p;

  p = serial0;

  result  = p[0];
  while ((result & 0x8000) == 0)
    result = p[0];
  return (char)(result & 0xff); 
}

int _read(int file, char *buf, int nbytes) {
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
