/* nash.h
   version 0.6
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#ifndef NASH_H
#define NASH_H 1

#include <stdint.h>

#include <gmp.h>

void init_gmp(unsigned int b);
void init_weight(unsigned int b, mpz_t k);
uint16_t standard_nash_weight();
uint16_t proth_nash_weight();

#endif // NASH_H
