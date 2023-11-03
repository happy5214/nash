# Nash Weight Calculators

This repository contains a set of Nash weight calculators. The Nash weight is a
measure used in prime-searching to estimate the density of primes for Proth and
Riesel *k* sequences. It is based on running a Nash sieve to depth *p*=256 for
an *n* range of length 10,000 (the exact range depends on which version of the
weight is used; these calculators support two). For more information on the Nash
weight, see [this Prime-Wiki page](https://www.rieselprime.de/ziki/Nash_weight).

The original versions of these calculators were published by Thomas Ritschel on
a [MersenneForum thread](https://www.mersenneforum.org/showthread.php?t=7213).
The source history of those versions is included as the `mersenne-forum` branch
of this repository. I have restructured the code by refactoring the common code
into the `nash_lib.c` module, and this code can be found in the `c/` folder. A
partial rewrite/port in C++ can be found in the `cpp/` folder, and this contains
a couple of speed improvements.

The license of this program is uncertain. I attempted to contact Ritschel
privately to clarify the license for his code, but I did not receive a response.
The original source was publicly shared, and I have no reason to believe the
author intended to restrict its use.
