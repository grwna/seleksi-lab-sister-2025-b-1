#pragma once
#include <stdio.h>
#include "symbols.h"
#include "operations.h"

uint string_len(char* str);
uint parse_big_int(char string[], int num[]);
void print_bignum(int num[], uint size);
