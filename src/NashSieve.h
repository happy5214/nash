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

class NashSieve {
public:
	NashSieve(const unsigned int base, const mpz_class k, const bool isRiesel);

	std::vector<bool> sieve(const unsigned int min, const unsigned int max) const;
	template <std::size_t sieveSize>
	std::bitset<sieveSize> sieve(const std::size_t min) const;

	unsigned int standard_nash_weight() const;
	unsigned int proth_nash_weight() const;

private:
	std::forward_list<std::pair<unsigned int, unsigned int>> factors;

	unsigned int calculate_nash_weight(const unsigned int min) const;
};

#endif // NASH_SIEVE_H
