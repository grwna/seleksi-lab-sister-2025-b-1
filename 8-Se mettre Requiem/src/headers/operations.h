#pragma once
#include "symbols.h"

ull add(ull a, ull b);
ull sub(ull a, ull b);
ull mult(ull a, ull b);
ull div(ull a, ull b);
ull mod(ull a, ull b);

ull barrett_reduce(ull x, ull prime, ull m);
