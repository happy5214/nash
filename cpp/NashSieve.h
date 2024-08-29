/* NashSieve.h
   version 0.8
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#ifndef NASH_SIEVE_H
#define NASH_SIEVE_H 1

#include <bitset>
#include <forward_list>
#include <utility>
#include <vector>

#include <gmpxx.h>

typedef std::pair< unsigned int, unsigned int > Factor;

class NashSieve {
public:
	NashSieve(const unsigned int base, const mpz_class k);

	std::vector<bool> sieve(const unsigned int min, const unsigned int max) const;
	template <std::size_t sieveSize>
	std::bitset<sieveSize> sieve(std::size_t min) const;

	const unsigned int standard_nash_weight() const;
	const unsigned int proth_nash_weight() const;

private:
	std::forward_list<Factor> factors;

	const unsigned int calculate_nash_weight(const unsigned int min) const;
};

#endif // NASH_SIEVE_H
