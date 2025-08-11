#pragma once
#include "symbols.h"

ull add(ull a, ull b);
ull sub(ull a, ull b);
ull mult(ull a, ull b);
ull div(ull a, ull b);
ull mod(ull a, ull b);

ull barrett_reduce(ull x, ull prime, ull m);
// log2
#define CHAR 0
#define INT 2
#define ULL 3
#define INDEX(arr_ptr, idx, elem_type, log2) (*((elem_type *)add((ull)(arr_ptr), (ull)(idx) << log2)))