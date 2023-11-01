/* NashSieve.h
   version 0.7
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#ifndef NASH_SIEVE_H
#define NASH_SIEVE_H 1

#include <bitset>
#include <utility>
#include <vector>

#include <gmpxx.h>

typedef std::pair< unsigned int, unsigned int > Factor;

class NashSieve {
public:
	NashSieve(unsigned int base, mpz_class k);

	std::vector<bool> sieve(unsigned int min, unsigned int max);
	template <std::size_t sieveSize>
	std::bitset<sieveSize> sieve(std::size_t min);

	unsigned int standard_nash_weight();
	unsigned int proth_nash_weight();

private:
	std::vector<Factor> factors;

	unsigned int calculate_nash_weight(unsigned int min);
};

#endif // NASH_SIEVE_H
