/* nash_lib.c
   version 0.6
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#include <math.h>

#include <gmp.h>

#include "nash.h"

typedef struct {
  uint16_t exponent;
  uint16_t skip;
} factor_t;

static mpz_t ptab[512];
static mpz_t z;
static factor_t factors[500];
static uint16_t factorCount;
static uint8_t sieveArea[SIEVE_AREA_SIZE];

void init_gmp(unsigned int b)
{
  mp_size_t bits = (mp_size_t) 1024*log(b)/log(2.0);  /* this is just a crude estimate */
  mpz_init(z);
  mpz_array_init(*ptab, 512, bits);     /* dummy init */
}

void init_weight(unsigned int b, mpz_t k)
{
  uint16_t n;

  mpz_set(z, k);

  for (n = 0; n <= 511; n++)
  {
/*    mpz_mul_ui(ptab[n], ptab[n-1], 2); */
    mpz_add_ui(ptab[n], z, 1);
    mpz_mul_ui(z, z, b);
/*    gmp_printf("%d*%d^%d-1 = %Zd\n", k, b, n, ptab[n]); */
  }

  uint16_t exponent, skip;
  factorCount = 0;
  for (skip = 1; skip <= 256; skip++)
    for (exponent = 0; exponent < skip; exponent++)
    {
      for (n = 0; n < factorCount; n++)
        if (skip % factors[n].skip == 0 && exponent % factors[n].skip == factors[n].exponent)
	  break;
      if (n >= factorCount)
      {
        mpz_gcd(z, ptab[exponent], ptab[exponent + skip]);
        if (mpz_cmp_ui(z, 1) > 0)
        {
          factors[factorCount].exponent = exponent;
          factors[factorCount].skip = skip;
          factorCount++;
       /*   printf("Eliminate %d, step %d\n", i, d); */
        }
      }
    }
}

uint16_t standard_nash_weight()
{
  uint16_t i;
  for (i = 0; i < SIEVE_AREA_SIZE; i++)
    sieveArea[i] = 1;

  uint32_t exponent;
  uint16_t skip;
  for (i = 0; i < factorCount; i++)
  {
    exponent = factors[i].exponent;
    skip = factors[i].skip;
    while (exponent < 110000)
    {
      if (exponent >= 100000)
        sieveArea[exponent - 100000] = 0;
      exponent += skip;
    }
  }  

  uint16_t sum = 0;
  for (i = 0; i < SIEVE_AREA_SIZE; i++)
    sum += sieveArea[i];

  return sum;
}

uint16_t proth_nash_weight()
{
  uint16_t i;
  for (i = 0; i < SIEVE_AREA_SIZE; i++)
    sieveArea[i] = 1;

  uint16_t exponent, skip;
  for (i = 0; i < factorCount; i++)
  {
    exponent = factors[i].exponent;
    skip = factors[i].skip;
    while (exponent < 10000)
    {
      sieveArea[exponent] = 0;
      exponent += skip;
    }
  }

  uint16_t sum = 0;
  for (i = 0; i < SIEVE_AREA_SIZE; i++)
    sum += sieveArea[i];

  return sum;
}
