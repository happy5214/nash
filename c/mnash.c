/* mnash.c
   version 0.6
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <gmp.h>

#include "nash.h"

static const char * invocation_name = "mnash";

int main(int argc, char* argv[])
{
  unsigned int b;
  uint16_t n, w;
  mpz_t k, kstart, kstop, kstep;
  int limit;
  int lsign = 1;
  int comp;
  mpz_t k, kstart, kstop, kstep;

  if (argc > 0)
    invocation_name = argv[0];

  if (argc < 2)
  {
    printf("%s - a tool for computing Nash weights for sequences k*b^n+-1\n\n", invocation_name);
    printf("usage: %s <kmin> <kmax> <kstep> <b> <limit>\n", invocation_name);
    printf("or:    %s <kmin> <kmax> <kstep>\n\n", invocation_name);
	printf("The parameters <b> and <limit> are optional.\n");
    printf("If no base <b> is given, b=2 is assumed.\n\n");
	printf("The parameter <limit> controls the printing:\n");
	printf("     weight <= limit will be printed,\n");
	printf("     weight >  limit will be omitted.\n");
	printf("If no limit is given, all weights will be printed.\n");
	printf("When looking for high weight sequences, the above rule can be reverted by using negative values for <limit>:\n");
	printf("     weight >= -limit will be printed,\n");
	printf("     weight <  -limit will be omitted.\n\n");
    printf("By default Proth sequences (k*b^n+1) are assumed.\n");
    printf("For Riesel sequences (k*b^n-1) enter k as -k.\n\n\n");
    printf("Example (computing the Nash weight for k*3^n-1 for k=10 to k=14):\n\n");
    printf("   %s -14 -10 2 3\n\n", invocation_name);
    printf("   -14  3 1524 1523\n");
    printf("   -12  3 2359 2369\n");
    printf("   -10  3 4054 4038\n");
    printf("The first two values are k and b, the third value (1524) is the\n");
    printf("standard Nash weight for the interval 100000 <= n < 110000.\n");
    printf("The forth value is the Nash weight for 0 <= n < 10000.\n");
    exit(1);
  }

  if (argc > 4)
    b = (unsigned int) atoi(argv[4]);
  else
    b = 2;

  if (argc > 5)
	limit = atoi(argv[5]);
  else
	limit = 10000;

  if (limit < 0)
  {
	lsign = -1;
	limit = -limit;
  }

  mpz_init_set_str(kstart, argv[1], 10);
  mpz_init_set_str(kstop, argv[2], 10);

  if (argc > 3)
    mpz_init_set_str(kstep, argv[3], 10);
  else
    mpz_init_set_ui(kstep, 2);

  mpz_init_set(k, kstart);
  comp = mpz_cmp(k, kstop);
  if (mpz_cmp(kstart, kstop) > 0)      // if kstart > kstop
    if (mpz_sgn(kstep) > 0)            // if kstep is positive
      mpz_neg(kstep, kstep);
  if (mpz_cmp(kstart, kstop) < 0)      // if kstart < kstop
    if (mpz_sgn(kstep) < 0)            // if kstep is negative
      mpz_neg(kstep, kstep);

  while (mpz_cmp(k, kstop)*comp >= 0)  // run until sign of comparison changes
  {
    init_nash_weight(b, k);
    n = standard_nash_weight();
    w = proth_nash_weight();
	if (((lsign == 1) && ((w <= limit) || (n <= limit))) || ((lsign == -1) && ((w >= limit) || (n >= limit))))
      gmp_printf("%15Zd %d %4" PRIu16 " %4" PRIu16 "\n", k, b, n, w);
    mpz_add(k, k, kstep);
  }
  return 0;
}
