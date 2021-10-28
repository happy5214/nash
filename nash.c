/* nash.c
   version 0.6
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmp.h>

#include "nash.h"

int main(int argc, char* argv[])
{
  unsigned int b, n, w;
  mpz_t k;
  if (argc < 2)
  {
    printf("%s - a tool for computing Nash weights for sequences k*b^n+-1\n\n", argv[0]);
    printf("usage: %s <k> <b>\n", argv[0]);
    printf("or:    %s <k>\n\n", argv[0]);
    printf("If no base <b> is given, b=2 is assumed.\n");
    printf("By default Proth sequences (k*b^n+1) are assumed.\n");
    printf("For Riesel sequences (k*b^n-1) enter k as -k.\n\n\n");
    printf("Example (computing the Nash weight for 14*17^n-1):\n\n");
    printf("   %s -14 17\n\n", argv[0]);
    printf("   -14 17  803  800\n\n");
    printf("The first two values are k and b, the third value (803) is the\n");
    printf("standard Nash weight for the interval 100000 <= n < 110000.\n");
    printf("The fourth value is the Nash weight for 0 <= n < 10000.\n");
    exit(1);
  }

  if (argc > 2)
    b = (unsigned int) atoi(argv[2]);
  else
    b = 2;

  init_gmp(b);

  mpz_init_set_str(k, argv[1], 10);

  init_weight(b, k);
  n = nash();
  w = weight();
  printf("%s %d %4d %4d\n", argv[1], b, n, w);
  return 0;
}
