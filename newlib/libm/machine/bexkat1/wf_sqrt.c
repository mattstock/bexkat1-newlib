#include <math.h>

float sqrtf(float in)
{
  asm("sqrt.s %12, %0\n");;
}
