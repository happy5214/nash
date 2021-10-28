/* nash.h
   version 0.6
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#include <gmp.h>

void init_gmp(unsigned int b);
void init_weight(unsigned int b, mpz_t k);
unsigned int nash();
unsigned int weight();
