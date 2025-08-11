#pragma once
#include <stdio.h>
#include "symbols.h"

uint string_len(const char* str);
uint parse_big_int(char string[], int num[]);
void print_bignum(int num[], uint size);
