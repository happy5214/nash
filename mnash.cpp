/* mnash.cpp
   version 0.7
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#include <cstdio>
#include <cstdlib>

#include <gmpxx.h>

#include "NashSieve.h"

static const char * invocation_name = "mnash";

int main(int argc, char* argv[]) {
	unsigned int base;
	unsigned int standardWeight, prothWeight;
	int comp;
	mpz_class k, kstart, kstop, kstep;

	if (argc > 0) {
		invocation_name = argv[0];
	}

	if (argc < 2) {
		printf("%s - a tool for computing Nash weights for sequences k*b^n+-1\n\n", invocation_name);
		printf("usage: %s <kmin> <kmax> <kstep> <b>\n", invocation_name);
		printf("or:    %s <kmin> <kmax> <kstep>\n\n", invocation_name);
		printf("If no base <b> is given, b=2 is assumed.\n");
		printf("By default Proth sequences (k*b^n+1) are assumed.\n");
		printf("For Riesel sequences (k*b^n-1) enter k as -k.\n\n\n");
		printf("Example (computing the Nash weight for k*3^n-1 for k=10 to k=14):\n\n");
		printf("   %s -14 -10 2 3\n\n", invocation_name);
		printf("   -14  3 1524 1523\n");
		printf("   -12  3 2359 2369\n");
		printf("   -10  3 4054 4038\n");
		printf("The first two values are k and b, the third value (1524) is the\n");
		printf("standard Nash weight for the interval 100000 <= n < 110000.\n");
		printf("The forth value is the Nash weight for 0 <= n < 10000.\n");
		return 1;
	}

	if (argc > 4) {
		base = (unsigned int) atoi(argv[4]);
	} else {
		base = 2;
	}

	kstart = argv[1];
	kstop = argv[2];

	if (argc > 3) {
		kstep = argv[3];
	} else {
		kstep = 2;
	}

	k = kstart;
	comp = cmp(k, kstop);

	int stepSign = sgn(kstep);
	if (stepSign == 0) {
		fprintf(stderr, "Cannot use step of 0. Exiting.\n");
		return 1;
	}

	if (cmp(kstart, kstop) == stepSign) {   // if kstart <=> kstop and kstep have same sign
		kstep = -kstep;
	}

	while (cmp(k, kstop) * comp >= 0) {  // run until sign of comparison changes
		NashSieve siever(base, k);
		standardWeight = siever.standard_nash_weight();
		prothWeight = siever.proth_nash_weight();
		gmp_printf("%15Zd %d %4u %4u\n", k, base, standardWeight, prothWeight);
		k += kstep;
	}

	return 0;
}
