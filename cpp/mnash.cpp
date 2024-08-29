/* mnash.cpp
   version 0.8
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <gmpxx.h>

#include "arg_parser.h"
#include "NashSieve.h"

static const char * invocation_name = "mnash";

void print_help() {
	printf("MNash - a tool for computing Nash weights for sequences k*b^n+-1\n\n");
	printf("usage: %s <kmin> <kmax> <kstep> <b>\n", invocation_name);
	printf("or:    %s <kmin> <kmax> <kstep>\n\n", invocation_name);
	printf("If no base <b> is given, b=2 is assumed.\n");
	printf("By default Proth sequences (k*b^n+1) are assumed.\n");
	printf("For Riesel sequences (k*b^n-1), add the -r parameter.\n\n\n");
	printf("Example (computing the Nash weight for k*3^n-1 for k=10 to k=14):\n\n");
	printf("   %s -r 10 14 2 3\n\n", invocation_name);
	printf("   10  3 4054 4038\n");
	printf("   12  3 2359 2369\n");
	printf("   14  3 1524 1523\n");
	printf("The first two values are k and b, the third value (1524) is the\n");
	printf("standard Nash weight for the interval 100000 <= n < 110000.\n");
	printf("The forth value is the Nash weight for 0 <= n < 10000.\n");
}

int main(int argc, char* argv[]) {
	if (argc > 0) {
		invocation_name = argv[0];
	}

	// Parse arguments
	const Arg_parser::Option options[] = {
		{ 'w', "minimum-weight", Arg_parser::yes },
		{ 'W', "maximum-weight", Arg_parser::yes },
		{ 'r', "riesel",         Arg_parser::no  },
		{ 'h', "help",           Arg_parser::no  },
		{   0, 0,                Arg_parser::no  }
	};

	const Arg_parser parser( argc, argv, options );
	if (parser.error().size()) {
		std::cerr << "Argument error: " << parser.error() << std::endl;
		return 1;
	}

	unsigned int minimumWeight = 0;
	unsigned int maximumWeight = 10000;
	bool isRiesel = false;

	int argind;

	for (argind = 0; argind < parser.arguments(); ++argind ) {
		const int code = parser.code(argind);
		if (!code) {
			break;
		}
		switch (code) {
			case 'w': minimumWeight = (unsigned int) std::stoi(parser.argument(argind)); break;
			case 'W': maximumWeight = (unsigned int) std::stoi(parser.argument(argind)); break;
			case 'r': isRiesel = true; break;
			case 'h': print_help(); return 0;
			default :
				std::cerr << "Uncaught option: " << code << std::endl;
		}
	} // end process options

	const std::string k_min = parser.argument( argind++ );
	if (k_min.empty()) {
		print_help();
		return 1;
	}
	const std::string k_max = parser.argument( argind++ );
	if (k_max.empty()) {
		print_help();
		return 1;
	}
	std::string k_step = parser.argument( argind++ );
	if (k_step.empty()) {
		k_step = "2";
	}
	std::string base_str = parser.argument( argind++ );
	if (base_str.empty()) {
		base_str = "2";
	}

	mpz_class k, kstart, kstop, kstep;
	kstart = k_min;
	kstop = k_max;
	kstep = k_step;
	k = kstart;

	const unsigned int base = (unsigned int) std::stoi(base_str);

	const int stepSign = sgn(kstep);
	if (stepSign == 0) {
		std::cerr << "Cannot use step of 0. Exiting." << std::endl;
		return 1;
	}

	if (cmp(kstart, kstop) == stepSign) {   // if kstart <=> kstop and kstep have same sign
		kstep = -kstep;
	}

	const int comp = cmp(k, kstop);

	while (cmp(k, kstop) * comp >= 0) {  // run until sign of comparison changes
		const NashSieve siever(base, isRiesel ? -k : k);
		const unsigned long int standardWeight = siever.standard_nash_weight();
		const unsigned long int prothWeight = siever.proth_nash_weight();
		if ((minimumWeight <= maximumWeight && (standardWeight >= minimumWeight && standardWeight <= maximumWeight)) ||
			(minimumWeight > maximumWeight && (standardWeight >= minimumWeight || standardWeight <= maximumWeight))) {
			gmp_printf("%15Zd %d %4u %4u\n", k, base, standardWeight, prothWeight);
		}
		k += kstep;
	}

	return 0;
}
