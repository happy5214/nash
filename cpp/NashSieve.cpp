/* NashSieve.cpp
   version 0.7
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#include <bitset>
#include <vector>

#include <gmpxx.h>

#include "NashSieve.h"

#define SIEVE_LIMIT 256
#define SIEVE_P_TEST_SIZE SIEVE_LIMIT * 2

NashSieve::NashSieve(unsigned int base, mpz_class k) {
	mpz_class pTable[SIEVE_P_TEST_SIZE];
	mpz_class temp = k;

	for (unsigned int n = 0; n < SIEVE_P_TEST_SIZE; n++) {
		/*    mpz_mul_ui(pTable[n], pTable[n-1], 2); */
		pTable[n] = temp + 1;
		temp *= base;
		/*    gmp_printf("%d*%d^%d-1 = %Zd\n", k, base, n, pTable[n]); */
	}

	unsigned int offset, modulus;
	for (modulus = 1; modulus <= SIEVE_LIMIT; modulus++) {
		std::bitset<SIEVE_LIMIT> offsets;
		offsets.set();
		for (const Factor& factor : factors) {
			if (modulus % factor.second == 0) {
				for (unsigned int j = factor.first; j < modulus; j += factor.second) {
					offsets.reset(j);
				}
			}
		}
		for (offset = 0; offset < modulus; offset++) {
			if (offsets.test(offset)) {
				temp = gcd(pTable[offset], pTable[offset + modulus]);
				if (temp > 1) {
					factors.push_front(std::make_pair(offset, modulus));
					/*   printf("Eliminate %d, step %d\n", i, d); */
				}
			}
		}
	}
}

std::vector<bool> NashSieve::sieve(unsigned int min, unsigned int max) {
	const unsigned int sieveSize = max - min;

	std::vector<bool> sieveData;
	sieveData.assign(sieveSize, true);

	unsigned int n, modulus;
	for (const Factor& factor : factors) {
		n = factor.first;
		modulus = factor.second;
		for (; n < min; n += modulus) {}
		for (; n < max; n += modulus) {
			sieveData[n - min] = false;
		}
	}

	return sieveData;
}

template <std::size_t sieveSize>
std::bitset<sieveSize> NashSieve::sieve(std::size_t min) {
	const std::size_t max = min + sieveSize;

	std::bitset<sieveSize> sieveData;
	sieveData.set();

	unsigned int n, modulus;
	for (const Factor& factor : factors) {
		n = factor.first;
		modulus = factor.second;
		for (; n < min; n += modulus) {}
		for (; n < max; n += modulus) {
			sieveData.reset(n - min);
		}
	}

	return sieveData;
}

unsigned int NashSieve::standard_nash_weight() {
	return calculate_nash_weight(100000);
}

unsigned int NashSieve::proth_nash_weight() {
	return calculate_nash_weight(0);
}

unsigned int NashSieve::calculate_nash_weight(unsigned int min) {
	std::bitset<10000> sieveData = sieve<10000>(min);

	return sieveData.count();
}
