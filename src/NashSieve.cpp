/* NashSieve.cpp
   version 0.8
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#include <array>
#include <bitset>
#include <stdexcept>
#include <vector>
#include <cstdlib>

#include <gmpxx.h>

#include "NashSieve.h"

template <std::size_t sieveLimit>
NashSieve<sieveLimit>::NashSieve(const unsigned int base, const mpz_class k, const bool isRiesel) {
	if (base < 2) {
		throw std::domain_error("base");
	}
	if (k == 0) {
		throw std::domain_error("k");
	}

	std::array<mpz_class, sieveLimit << 2> pTable;
	mpz_class temp = isRiesel ? -k : k;

	for (auto&& tableEntry : pTable) {
		tableEntry = temp + 1;
		temp *= base;
	}

	for (unsigned int modulus = 1; modulus <= sieveLimit; modulus++) {
		std::bitset<sieveLimit> offsets;

		for (const auto [factorOffset, factorModulus] : factors) {
			if (modulus % factorModulus == 0) {
				for (unsigned int n = factorOffset; n < modulus; n += factorModulus) {
					offsets.set(n);
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

template <std::size_t sieveLimit>
std::vector<bool> NashSieve<sieveLimit>::sieve(const unsigned int min, const unsigned int max) const {
	const unsigned int sieveSize = max - min;

	std::vector<bool> sieveData;
	sieveData.assign(sieveSize, true);

	for (const auto [offset, modulus] : factors) {
		unsigned int n = offset;
		for (; n < min; n += modulus) {}
		for (; n < max; n += modulus) {
			sieveData[n - min] = false;
		}
	}

	return sieveData;
}

template <std::size_t sieveLimit>
template <std::size_t sieveSize>
std::bitset<sieveSize> NashSieve<sieveLimit>::sieve(const std::size_t min) const {
	const std::size_t max = min + sieveSize;

	std::bitset<sieveSize> sieveData;
	sieveData.set();

	for (const auto [offset, modulus] : factors) {
		unsigned int n = offset;
		for (; n < min; n += modulus) {}
		for (; n < max; n += modulus) {
			sieveData.reset(n - min);
		}
	}

	return sieveData;
}

template <std::size_t sieveLimit>
unsigned int NashSieve<sieveLimit>::standard_nash_weight() const {
	return calculate_nash_weight(100000);
}

template <std::size_t sieveLimit>
unsigned int NashSieve<sieveLimit>::proth_nash_weight() const {
	return calculate_nash_weight(0);
}

template <std::size_t sieveLimit>
unsigned int NashSieve<sieveLimit>::calculate_nash_weight(const unsigned int min) const {
	const std::bitset<10000> sieveData = sieve<10000>(min);

	return sieveData.count();
}
