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
#define SIEVE_P_TEST_SIZE SIEVE_LIMIT << 2

NashSieve::NashSieve(unsigned int base, mpz_class k) {
	mpz_class pTable[SIEVE_P_TEST_SIZE];
	mpz_class temp = k;

	for (unsigned int n = 0; n < SIEVE_P_TEST_SIZE; n++) {
		/*    mpz_mul_ui(pTable[n], pTable[n-1], 2); */
		pTable[n] = temp + 1;
		temp *= base;
		/*    gmp_printf("%d*%d^%d-1 = %Zd\n", k, base, n, pTable[n]); */
	}

	unsigned int exponent, skip;
	for (skip = 1; skip <= SIEVE_LIMIT; skip++) {
		std::bitset<SIEVE_LIMIT> offsets;
		offsets.set();
		for (std::vector<Factor>::size_type i = 0; i < factors.size(); i++) {
			if (skip % factors[i].second == 0) {
				for (unsigned int j = factors[i].first; j < SIEVE_LIMIT; j += factors[i].second) {
					offsets.reset(j);
				}
			}
		}
		for (exponent = 0; exponent < skip; exponent++) {
			if (offsets.test(exponent)) {
				temp = gcd(pTable[exponent], pTable[exponent + skip]);
				if (temp > 1) {
					factors.push_back(std::make_pair(exponent, skip));
					for (unsigned int j = exponent; j < SIEVE_LIMIT; j += skip) {
						offsets.reset(j);
					}
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

	unsigned int exponent, skip;
	for (std::vector<Factor>::size_type i = 0; i < factors.size(); i++) {
		exponent = factors[i].first;
		skip = factors[i].second;
		while (exponent < max) {
			if (exponent >= min) {
				sieveData[exponent - min] = false;
			}
			exponent += skip;
		}
	}

	return sieveData;
}

template <std::size_t sieveSize>
std::bitset<sieveSize> NashSieve::sieve(std::size_t min) {
	const std::size_t max = min + sieveSize;

	std::bitset<sieveSize> sieveData;
	sieveData.set();

	unsigned int exponent, skip;
	for (std::vector<Factor>::size_type i = 0; i < factors.size(); i++) {
		exponent = factors[i].first;
		skip = factors[i].second;
		while (exponent < max) {
			if (exponent >= min) {
				sieveData.reset(exponent - min);
			}
			exponent += skip;
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
