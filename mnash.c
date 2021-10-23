/* nash.c
   version 0.5
   by Thomas Ritschel
   based on Jack Brennen's Java applet
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <gmp.h>

mpz_t ptab[512];
mpz_t z;
int evalue[500];
int dvalue[500];
int sva[10000];
int ecnt;

void init_gmp(unsigned int b)
{
  mp_size_t bits = (unsigned int) 1024.0*log(b)/log(2.0);  /* this is just a crude estimate */
  mpz_init(z);
  mpz_array_init(*ptab, 512, bits);     /* dummy init */
}

void init_weight(unsigned int b, long k)
{
  unsigned int d, i, n;

  mpz_set_si(z, k);

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

int nash()
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

  return(n);
}

int weight()
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

  return(n);
}

int main(int argc, char* argv[])
{
  unsigned int b;
  long k;
  long kmin, kmax, kstep;
  char *rest;
  int n;
  int w;
  if (argc < 2)
  {
    printf("%s - a tool for computing Nash weights for sequences k*b^n+-1\n\n", argv[0]);
    printf("usage: %s <kmin> <kmax> <kstep> <b>\n", argv[0]);
    printf("or:    %s <kmin> <kmax> <kstep>\n\n", argv[0]);
    printf("If no base <b> is given, b=2 is assumed.\n");
    printf("By default Proth sequences (k*b^n+1) are assumed.\n");
    printf("For Riesel sequences (k*b^n-1) enter k as -k.\n\n\n");
    printf("Example (computing the Nash weight for 14*17^n-1):\n\n");
    printf("   %s -14 17\n\n", argv[0]);
    printf("   -14 17  803  800\n\n");
    printf("The first two values are k and b, the third value (803) is the\n");
    printf("standard Nash weight for the interval 100000 <= n < 110000.\n");
    printf("The forth value is the Nash weight for 0 <= n < 10000.\n");
    exit(1);
  }

  if (argc > 4)
    b = (unsigned int) atoi(argv[4]);
  else
    b = 2;

  kmin = strtol(argv[1], &rest, 10);
  kmax = strtol(argv[2], &rest, 10);
  kstep = strtol(argv[3], &rest, 10);

  init_gmp(b);

  for (k=kmin; k<=kmax; k+=kstep)
  {
    init_weight(b, k);
    n = nash();
    w = weight();
    printf("%15ld %d %4d %4d\n", k, b, n, w);
  }
  return(0);
}
