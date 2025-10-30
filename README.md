# Nash Weight Calculators

This repository contains a set of Nash weight calculators. The Nash weight is a
measure used in prime-searching to estimate the density of primes for Proth and
Riesel *k* sequences. It is based on running a Nash sieve to depth *p*=256 for
an *n* range of length 10,000 (the exact range depends on which version of the
weight is used; these calculators support two). For more information on the Nash
weight, see [this Prime-Wiki page](https://www.rieselprime.de/ziki/Nash_weight).

The original versions of these calculators were published by Thomas Ritschel on
a [MersenneForum thread](https://www.mersenneforum.org/node/5603).
They were based on a Java applet coded by Jack Brennen (code available at the
[Wayback Machine](https://web.archive.org/web/20230421222216/https://www.brennen.net/primes/ProthWeight.java)).
The source history of those versions is included as the `mersenne-forum` branch
of this repository. I ported this code to C++, and this code includes a couple
of speed improvements.

This program is licensed under the MIT License, except for the `Arg_parser`
code, which is under a 2-clause BSD license.
