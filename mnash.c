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
unsigned int ecnt;

void init_gmp(unsigned int b)
{
  mp_size_t bits = (mp_size_t) 1024.0*log(b)/log(2.0);  /* this is just a crude estimate */
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
  mpz_t k, kstart, kstop, kstep;
  int n;
  int w;
  int comp;
  if (argc < 2)
  {
    printf("%s - a tool for computing Nash weights for sequences k*b^n+-1\n\n", argv[0]);
    printf("usage: %s <kmin> <kmax> <kstep> <b>\n", argv[0]);
    printf("or:    %s <kmin> <kmax> <kstep>\n\n", argv[0]);
    printf("If no base <b> is given, b=2 is assumed.\n");
    printf("By default Proth sequences (k*b^n+1) are assumed.\n");
    printf("For Riesel sequences (k*b^n-1) enter k as -k.\n\n\n");
    printf("Example (computing the Nash weight for k*3^n-1 for k=10 to k=14):\n\n");
    printf("   %s -14 -10 2 3\n\n", argv[0]);
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

  mpz_init_set_str(kstart, argv[1], 10);
  mpz_init_set_str(kstop, argv[2], 10);

  if (argc > 3)
    mpz_init_set_str(kstep, argv[3], 10);
  else
    mpz_init_set_ui(kstep, 2);

  init_gmp(b);

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
    init_weight(b, k);
    n = nash();
    w = weight();
    gmp_printf("%15Zd %d %4d %4d\n", k, b, n, w);
    mpz_add(k, k, kstep);
  }
  return(0);
}
