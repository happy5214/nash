/* NashSieve.cpp
   version 0.7
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

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
		for (exponent = 0; exponent < skip; exponent++) {
			bool foundFactor = false;
			for (std::vector<Factor>::size_type i = 0; i < factors.size(); i++) {
				if (skip % factors[i].second == 0 && exponent % factors[i].second == factors[i].first) {
					foundFactor = true;
					break;
				}
			}
			if (!foundFactor) {
				temp = gcd(pTable[exponent], pTable[exponent + skip]);
				if (temp > 1) {
					factors.push_back(std::make_pair(exponent, skip));
					/*   printf("Eliminate %d, step %d\n", i, d); */
				}
			}
		}
	}
}

std::vector<bool> NashSieve::sieve(unsigned int min, unsigned int max) {
	unsigned int sieveSize = max - min;
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

unsigned int NashSieve::standard_nash_weight() {
	return calculate_nash_weight(100000, 110000);
}

unsigned int NashSieve::proth_nash_weight() {
	return calculate_nash_weight(0, 10000);
}

unsigned int NashSieve::calculate_nash_weight(unsigned int min, unsigned int max) {
	std::vector<bool> sieveData = sieve(min, max);

	unsigned int sum = 0;
	for (std::vector<bool>::size_type i = 0; i < sieveData.size(); i++) {
		if (sieveData[i]) {
			sum++;
		}
	}

	return sum;
}
