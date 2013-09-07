#include "sim.h"

Strategy::Strategy(int nswe, int nsweTest, int nsweFix) : nSWE(nswe), nSWETesting(nsweTest), nSWEFixing(nsweFix) { }

Strategy::Strategy(const Strategy& s) : nSWE(s.nSWE), nSWETesting(s.nSWETesting), nSWEFixing(s.nSWEFixing) { }
