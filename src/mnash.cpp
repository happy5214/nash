/* mnash.cpp
   version 0.8
   by Thomas Ritschel and Alexander Jones
   based on Jack Brennen's Java applet
*/

#include <iostream>
#include <stdexcept>
#include <string>

#include <gmpxx.h>

#include "arg_parser.h"
#include "NashSieve.h"

static std::string invocation_name = "mnash";

static void print_help() {
	std::cout << "MNash - a tool for computing Nash weights for sequences k*b^n+-1\n\n";
	std::cout << "usage: " << invocation_name << " <kmin> <kmax> <kstep> <b>\n";
	std::cout << "or:    " << invocation_name << " <kmin> <kmax> <kstep>\n\n";
	std::cout << "If no base <b> is given, b=2 is assumed.\n";
	std::cout << "By default Proth sequences (k*b^n+1) are assumed.\n";
	std::cout << "For Riesel sequences (k*b^n-1), add the -r parameter.\n\n\n";
	std::cout << "Example (computing the Nash weight for k*3^n-1 for k=10 to k=14):\n\n";
	std::cout << "   " << invocation_name << " -r 10 14 2 3\n\n";
	std::cout << "   10  3 4054 4038\n";
	std::cout << "   12  3 2359 2369\n";
	std::cout << "   14  3 1524 1523\n";
	std::cout << "The first two values are k and b, the third value (1524) is the\n";
	std::cout << "standard Nash weight for the interval 100000 <= n < 110000.\n";
	std::cout << "The fourth value is the Nash weight for 0 <= n < 10000.\n";
}

int main(const int argc, const char * const argv[]) {
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

	const mpz_class kstart(k_min);
	const mpz_class kstop(k_max);
	mpz_class kstep(k_step);

	const unsigned int base = (unsigned int) std::stoi(base_str);

	const int stepSign = sgn(kstep);
	if (stepSign == 0) {
		std::cerr << "Cannot use step of 0. Exiting." << std::endl;
		return 1;
	}

	int comp = cmp(kstart, kstop);

	if (comp == stepSign) {   // if kstart <=> kstop and kstep have same sign
		kstep = -kstep;
	} else if (comp == 0) {
		comp = -stepSign;
	}

	for (mpz_class k = kstart; cmp(k, kstop) * comp >= 0; k += kstep) {  // run until sign of comparison changes
		if (k == 0) {
			continue;
		}
		try {
			const StandardNashSieve siever(base, k, isRiesel);
			const unsigned int standardWeight = siever.standard_nash_weight();
			const unsigned int prothWeight = siever.proth_nash_weight();
			if ((minimumWeight <= maximumWeight && (standardWeight >= minimumWeight && standardWeight <= maximumWeight)) ||
				(minimumWeight > maximumWeight && (standardWeight >= minimumWeight || standardWeight <= maximumWeight))) {
				std::cout << k << " " << base << " " << standardWeight << " " << prothWeight << "\n";
			}
		} catch (const std::domain_error& e) {
			std::cerr << "Supplied value of parameter \"" << e.what() << "\" cannot be used for calculating Nash weights. Exiting..." << std::endl;
			return 1;
		}
	}

	return 0;
}
