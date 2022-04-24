/* nash.h
   version 0.6
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#ifndef NASH_H
#define NASH_H 1

#include <stdint.h>

#include <gmp.h>

#define SIEVE_AREA_SIZE 10000
#define SIEVE_P_TEST_SIZE 512

typedef struct {
  uint16_t exponent;
  uint16_t skip;
} factor_t;

void init_nash_weight(unsigned int b, mpz_t k);
uint16_t standard_nash_weight();
uint16_t proth_nash_weight();

#endif // NASH_H
