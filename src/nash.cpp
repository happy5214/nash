/* nash.cpp
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

static std::string invocation_name = "nash";

static void print_help() {
	std::cout << "Nash - a tool for computing Nash weights for sequences k*b^n+-1\n\n";
	std::cout << "usage: " << invocation_name << " <k> <b>\n";
	std::cout << "or:    " << invocation_name << " <k>\n\n";
	std::cout << "If no base <b> is given, b=2 is assumed.\n";
	std::cout << "By default Proth sequences (k*b^n+1) are assumed.\n";
	std::cout << "For Riesel sequences (k*b^n-1), add the -r parameter.\n\n\n";
	std::cout << "Example (computing the Nash weight for 14*17^n-1):\n\n";
	std::cout << "   " << invocation_name << " -r 14 17\n\n";
	std::cout << "   14 17  803  800\n\n";
	std::cout << "The first two values are k and b, the third value (803) is the\n";
	std::cout << "standard Nash weight for the interval 100000 <= n < 110000.\n";
	std::cout << "The fourth value is the Nash weight for 0 <= n < 10000.\n";
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

	try {
		const StandardNashSieve siever(base, k, isRiesel);
		const unsigned int standardWeight = siever.standard_nash_weight();
		const unsigned int prothWeight = siever.proth_nash_weight();
		std::cout << k << " " << base << " "  << standardWeight << " "  << prothWeight << std::endl;
	} catch (const std::domain_error& e) {
		std::cerr << "Supplied value of parameter \"" << e.what() << "\" cannot be used for calculating Nash weights. Exiting..." << std::endl;
		return 1;
	}

	return 0;
}
