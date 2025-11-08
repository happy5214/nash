#include "NashSieve.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	unsigned int standard;
	unsigned int proth;
} nash_weights_t;

nash_weights_t nash_weights(const char * const k_str, const unsigned int base, const int is_riesel);

#ifdef __cplusplus
} /* extern "C" */
#endif

#include <gmpxx.h>

nash_weights_t nash_weights(const char * const k_str, const unsigned int base, const int is_riesel) {
	nash_weights_t weights;

	const mpz_class k(k_str);
	const NashSieve siever(base, k, is_riesel);
	const unsigned int standardWeight = siever.standard_nash_weight();
	const unsigned int prothWeight = siever.proth_nash_weight();

	weights.standard = standardWeight;
	weights.proth = prothWeight;

	return weights;
}
