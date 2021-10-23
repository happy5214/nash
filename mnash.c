/* nash.c
   version 0.3
   by Thomas Ritschel (ritschel@chem.uni-potsdam.de)

   based on Jack Brennen's Java applet
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmp.h>

mpz_t ptab[512];
mpz_t z;
int evalue[500];
int dvalue[500];
int sva[10000];
int ecnt;

void init_gmp(void)
{
  mpz_init(z);
  mpz_array_init(*ptab, 512, 640);  /* dummy-init */
}

void init_weight(unsigned long k)
{
  unsigned int d, i, n;

  mpz_set_ui(z, k);

  for (n=0; n<=511; n++)
  {
/*    mpz_mul_ui(ptab[n], ptab[n-1], 2); */
    mpz_sub_ui(ptab[n], z, 1);
    mpz_mul_2exp(z, z, 1);
/*    gmp_printf("%d*2^%d-1 = %Zd\n", k, n, ptab[n]); */
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
  unsigned long k;
  unsigned long kmin, kmax, kstep;
  char *rest;
  int n;
  int w;
  if (argc < 4)
  {
    printf("usage: %s <kmin> <kmax> <kstep>\n\n\n", argv[0]);
    exit(1);
  }

  kmin = strtoul(argv[1], &rest, 10);
  kmax = strtoul(argv[2], &rest, 10);
  kstep = strtoul(argv[3], &rest, 10); 
/*  kstep = (unsigned long) atol(argv[3]); */

  init_gmp();

  for (k=kmin; k<=kmax; k+=kstep)
  {
/* if we want to throw away k's divisable by small primes: */
/*    if ((k%3) && (k%5)) */
/*    if ((k%3) && (k%5) && (k%7) && (k%11) && (k%13)) */
/*    if ((k%3) && (k%5) && (k%7)) 
    { */
      init_weight(k);
      n = nash();
      w = weight();
      printf("%15u %4d %4d\n", k, n, w);
/*    } */
  }
  return(0);
}
