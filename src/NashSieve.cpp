/* NashSieve.cpp
   version 0.8
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#include <bitset>
#include <vector>

#include <gmpxx.h>

#include "NashSieve.h"

#define SIEVE_LIMIT 256
#define SIEVE_P_TEST_SIZE SIEVE_LIMIT * 2

NashSieve::NashSieve(const unsigned int base, const mpz_class k, const bool isRiesel) {
	mpz_class pTable[SIEVE_P_TEST_SIZE];
	mpz_class temp = isRiesel ? -k : k;

	for (unsigned int n = 0; n < SIEVE_P_TEST_SIZE; n++) {
		pTable[n] = temp + 1;
		temp *= base;
	}

	for (unsigned int modulus = 1; modulus <= SIEVE_LIMIT; modulus++) {
		std::bitset<SIEVE_LIMIT> offsets;
		for (const Factor& factor : factors) {
			if (modulus % factor.second == 0) {
				for (unsigned int j = factor.first; j < modulus; j += factor.second) {
					offsets.set(j);
				}
			}
		}
		for (unsigned int offset = 0; offset < modulus; offset++) {
			if (!offsets.test(offset)) {
				temp = gcd(pTable[offset], pTable[offset + modulus]);
				if (temp > 1) {
					factors.push_front(std::make_pair(offset, modulus));
				}
			}
		}
	}
}

std::vector<bool> NashSieve::sieve(const unsigned int min, const unsigned int max) const {
	const unsigned int sieveSize = max - min;

	std::vector<bool> sieveData;
	sieveData.assign(sieveSize, true);

	for (const Factor& factor : factors) {
		unsigned int n = factor.first;
		const unsigned int modulus = factor.second;
		for (; n < min; n += modulus) {}
		for (; n < max; n += modulus) {
			sieveData[n - min] = false;
		}
	}

	return sieveData;
}

template <std::size_t sieveSize>
std::bitset<sieveSize> NashSieve::sieve(const std::size_t min) const {
	const std::size_t max = min + sieveSize;

	std::bitset<sieveSize> sieveData;
	sieveData.set();

	for (const Factor& factor : factors) {
		unsigned int n = factor.first;
		const unsigned int modulus = factor.second;
		for (; n < min; n += modulus) {}
		for (; n < max; n += modulus) {
			sieveData.reset(n - min);
		}
	}

	return sieveData;
}

const unsigned int NashSieve::standard_nash_weight() const {
	return calculate_nash_weight(100000);
}

const unsigned int NashSieve::proth_nash_weight() const {
	return calculate_nash_weight(0);
}

const unsigned int NashSieve::calculate_nash_weight(const unsigned int min) const {
	const std::bitset<10000> sieveData = sieve<10000>(min);

	return sieveData.count();
}
