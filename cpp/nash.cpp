/* nash.cpp
   version 0.7
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#include <cstdio>
#include <cstdlib>

#include <gmpxx.h>

#include "NashSieve.h"

static const char * invocation_name = "nash";

int main(int argc, char* argv[]) {
	unsigned int base;
	unsigned int standardWeight, prothWeight;
	mpz_class k;

	if (argc > 0) {
		invocation_name = argv[0];
	}

	if (argc < 2) {
		printf("%s - a tool for computing Nash weights for sequences k*b^n+-1\n\n", invocation_name);
		printf("usage: %s <k> <b>\n", invocation_name);
		printf("or:    %s <k>\n\n", invocation_name);
		printf("If no base <b> is given, b=2 is assumed.\n");
		printf("By default Proth sequences (k*b^n+1) are assumed.\n");
		printf("For Riesel sequences (k*b^n-1) enter k as -k.\n\n\n");
		printf("Example (computing the Nash weight for 14*17^n-1):\n\n");
		printf("   %s -14 17\n\n", invocation_name);
		printf("   -14 17  803  800\n\n");
		printf("The first two values are k and b, the third value (803) is the\n");
		printf("standard Nash weight for the interval 100000 <= n < 110000.\n");
		printf("The fourth value is the Nash weight for 0 <= n < 10000.\n");
		return 1;
	}

	if (argc > 2) {
		base = (unsigned int) atoi(argv[2]);
	} else {
		base = 2;
	}

	k = argv[1];

	NashSieve siever(base, k);
	standardWeight = siever.standard_nash_weight();
	prothWeight = siever.proth_nash_weight();
	printf("%s %u %4u %4u\n", argv[1], base, standardWeight, prothWeight);

	return 0;
}
