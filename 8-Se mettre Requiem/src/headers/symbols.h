#pragma once

typedef unsigned long long ull;
typedef unsigned int uint;
typedef long long ll;

// forward declarations
ull add(ull a, ull b);
ull sub(ull a, ull b);

// MACROS
#define EQUAL(A, B) (!((A) ^ (B)))
#define NOT_EQUAL(A, B) (((A) ^ (B)))


// found from here: https://stackoverflow.com/questions/10096599/bitwise-operations-equivalent-of-greater-than-operator

int GREATER(int a, int b);
int GREATER_OR_EQUAL(int a, int b);
int LESSER(int a, int b);
int LESSER_OR_EQUAL(int a, int b);
int LESSER_ULL(ull a, ull b); // USed once in mult_high