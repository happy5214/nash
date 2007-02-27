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

void my_test(void)
{
  unsigned long n;
  mpz_t k, z;
  mpz_init(z);
  mpz_init_set_str(k, "3", 10);
  n = 31;
  mpz_mul_2exp(z, k, n); 
  mpz_sub_ui(z, z, 1);
  gmp_printf("%Zd*2^%d-1 = %Zd\n", k, n, z);
}

void init_gmp(void)
{
  mpz_init(z);
  mpz_array_init(*ptab, 512, 640);  /* dummy-Initialisierung */
}

void init_weight(char *k)
{
  unsigned int d, i, n;

  mpz_init_set_str(z, k, 10);
//  mpz_set_ui(z, k);

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
//  unsigned long k;
  int n;
  int w;
  if (argc < 2)
  {
    printf("usage: %s <k>\n\n\n", argv[0]);
    exit(1);
  }

//  k = (unsigned long) atol(argv[1]);

  init_gmp();

  init_weight(argv[1]);
  n = nash();
  w = weight();
  printf("%15s %4d %4d\n", argv[1], n, w);
  return(0);
}
