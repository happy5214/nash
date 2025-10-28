/* nash.cpp
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

static const char * invocation_name = "nash";

static void print_help() {
	printf("Nash - a tool for computing Nash weights for sequences k*b^n+-1\n\n");
	printf("usage: %s <k> <b>\n", invocation_name);
	printf("or:    %s <k>\n\n", invocation_name);
	printf("If no base <b> is given, b=2 is assumed.\n");
	printf("By default Proth sequences (k*b^n+1) are assumed.\n");
	printf("For Riesel sequences (k*b^n-1), add the -r parameter.\n\n\n");
	printf("Example (computing the Nash weight for 14*17^n-1):\n\n");
	printf("   %s -r 14 17\n\n", invocation_name);
	printf("   14 17  803  800\n\n");
	printf("The first two values are k and b, the third value (803) is the\n");
	printf("standard Nash weight for the interval 100000 <= n < 110000.\n");
	printf("The fourth value is the Nash weight for 0 <= n < 10000.\n");
}

int main(const int argc, const char * const argv[]) {
	if (argc > 0) {
		invocation_name = argv[0];
	}

	// Parse arguments
	const Arg_parser::Option options[] = {
		{ 'r', "riesel",         Arg_parser::no  },
		{ 'h', "help",           Arg_parser::no  },
		{   0, 0,                Arg_parser::no  }
	};

	const Arg_parser parser( argc, argv, options );
	if (parser.error().size()) {
		std::cerr << "Argument error: " << parser.error() << std::endl;
		return 1;
	}

	bool isRiesel = false;

	int argind;

	for (argind = 0; argind < parser.arguments(); ++argind ) {
		const int code = parser.code(argind);
		if (!code) {
			break;
		}
		switch (code) {
			case 'r': isRiesel = true; break;
			case 'h': print_help(); return 0;
			default :
				std::cerr << "Uncaught option: " << code << std::endl;
		}
	} // end process options

	const std::string k_str = parser.argument( argind++ );
	if (k_str.empty()) {
		print_help();
		return 1;
	}
	std::string base_str = parser.argument( argind++ );
	if (base_str.empty()) {
		base_str = "2";
	}

	const unsigned int base = (unsigned int) std::stoi(base_str);
	const mpz_class k(k_str);

	const int kSign = sgn(k);
	if (kSign == 0) {
		std::cerr << "k cannot be 0" << std::endl;
		return 1;
	}

	const NashSieve siever(base, isRiesel ? -k : k);
	const unsigned int standardWeight = siever.standard_nash_weight();
	const unsigned int prothWeight = siever.proth_nash_weight();
	printf("%s %u %4u %4u\n", k_str.c_str(), base, standardWeight, prothWeight);

	return 0;
}
