/* nash_lib.c
   version 0.6
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#include <math.h>
#include <gmp.h>

#include "nash.h"

static mpz_t ptab[512];
static mpz_t z;
static unsigned int evalue[500];
static unsigned int dvalue[500];
static unsigned int sva[10000];
static unsigned int ecnt;

void init_gmp(unsigned int b)
{
  mp_size_t bits = (mp_size_t) 1024*log(b)/log(2.0);  /* this is just a crude estimate */
  mpz_init(z);
  mpz_array_init(*ptab, 512, bits);     /* dummy init */
}

void init_weight(unsigned int b, mpz_t k)
{
  unsigned int d, i, n;

  mpz_set(z, k);

  for (n=0; n<=511; n++)
  {
/*    mpz_mul_ui(ptab[n], ptab[n-1], 2); */
    mpz_add_ui(ptab[n], z, 1);
    mpz_mul_ui(z, z, b);
/*    gmp_printf("%d*%d^%d-1 = %Zd\n", k, b, n, ptab[n]); */
  }

  ecnt = 0;
  for (d=1; d<=256; d++)
    for (i=0; i<d; i++)
    {
      for (n=0; n<ecnt; n++)
        if (d%dvalue[n] == 0 && i%dvalue[n] == evalue[n])
	  break;
      if (n >= ecnt)
      {
        mpz_gcd(z, ptab[i], ptab[i+d]);
        if (mpz_cmp_ui(z, 1) > 0)
        {
          evalue[ecnt] = i;
          dvalue[ecnt] = d;
          ecnt++;
       /*   printf("Eliminate %d, step %d\n", i, d); */
        }
      }
    }
}

unsigned int nash()
{
  unsigned int i, d, n;
  for (i=0; i<10000; i++)
    sva[i] = 1;

  for (n=0; n<ecnt; n++)
  {
    i = evalue[n];
    d = dvalue[n];
    while (i<110000)
    {
      if (i >= 100000)
        sva[i-100000] = 0;
      i += d;
    }
  }  

  n = 0;
  for (i=0; i<10000; i++)
    n += sva[i];

  return n;
}

unsigned int weight()
{
  unsigned int i, d, n;
  for (i=0; i<10000; i++)
    sva[i] = 1;

  for (n=0; n<ecnt; n++)
  {
    i = evalue[n];
    d = dvalue[n];
    while (i<10000)
    {
      sva[i] = 0;
      i += d;
    }
  }

  n = 0;
  for (i=0; i<10000; i++)
    n += sva[i];

  return n;
}
