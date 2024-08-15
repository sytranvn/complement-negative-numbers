#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#ifndef DEBUG
#define DEBUG 0
#endif
#define debug_print(fmt, ...) \
            do { if (DEBUG) { fprintf(stderr, "debug: "); fprintf(stderr, fmt, __VA_ARGS__); } } while (0)

#define BITS 8
typedef int16_t number_t;
void print_bits(int* bits);

// faster than loop
#define flip_bits(n, b) ((n)^((1u << (b)) -1))

number_t flip_bits_loop(number_t n) {
	for (int i = 0; i < BITS; ++i)
		n ^= (1 << i);
	return n;
}

int sign_bit(number_t num) {
	return ((1u << (BITS-1)) & num) >> (BITS - 1);
}

int* bits_of_num(number_t num) {
	int* pbits = malloc(BITS);
	int i = BITS;
	while (i--) {
		pbits[i] = num & 1;
		num >>= 1;
	}
	return pbits;
}

char* to_decimal_1comp(number_t num) {
	char *pd = calloc(10, sizeof(char));
	int sign = sign_bit(num);
	if (sign) {
		pd[0] = '-';
		num = flip_bits(num, BITS);
	}

	sprintf(pd+sign, "%d", num);
	debug_print("%d", num);
	return pd;
}
number_t parse_1comp(const char* in_num) {
	number_t num = 0b0;
	size_t len = strlen(in_num);
	if (!len) return 0;
	int sign = in_num[0] == '-'?  1 : 0;
	for (int i = sign; i < len; ++i) {
		num = num*10 + (in_num[i] - '0');
	}
	if (sign) {
		num = flip_bits(num, BITS);
	}
	return num;
}

number_t parse_2comp(const char* in_num) {
	number_t num = 0b0;
	size_t len = strlen(in_num);
	if (!len) return 0;
	int sign = in_num[0] == '-'?  1 : 0;
	for (int i = sign; i < len; ++i) {
		num = num*10 + (in_num[i] - '0');
	}
	if (sign) {
		num = flip_bits(num, BITS) + 1;
	}
	return num;
}

char* to_decimal_2comp(number_t num) {
	char *pd = calloc(5, sizeof(char)); // 1 sign + 3 digits + '\0'
	int sign = sign_bit(num);
	if (sign) {
		pd[0] = '-';
		num = flip_bits(num, BITS) + 1;
	}
	sprintf(pd+sign, "%d", num);
	debug_print("%d", num);
	return pd;
}

char* to_decimal(number_t num, int comp) {
	char * presult = NULL;
	if (comp == 1) {
		presult = to_decimal_1comp(num);
	} else if (comp == 2) {
		presult = to_decimal_2comp(num);
	} 
	return presult;
}


void print_bits(int* bits) {
	for (int i = 0; i<BITS; ++i) {
		printf("%d", bits[i]);
		if (i % 4 == 3) printf(" ");
	}
	printf("\n");
}

int main() {
	number_t zero = 0b0;
	number_t one = 0b1;
	number_t nine = 0b00001001;
	number_t nnine_1comp = 0b11110110;
	number_t one_two_seven = 0b0111'1111;
	number_t neg_one_two_seven_2comp = 0b10000001;
	number_t min_2comp = 0b11111111;
	int8_t x = 0b11111111;
	number_t neg_fifteen = 0b11110001;


	print_bits(bits_of_num(zero));
	print_bits(bits_of_num(one));
	print_bits(bits_of_num(nnine_1comp));
	printf("%s\n", to_decimal(zero, 1));
	printf("%s\n", to_decimal(one, 1));
	printf("%s\n", to_decimal(nine, 1));
	printf("%s\n", to_decimal(nnine_1comp, 1));

	number_t v = parse_1comp("-9");
	print_bits(bits_of_num(v));
	assert(v == nnine_1comp);

	printf("2 complement number: \n");
	printf("-127 is %4s\n", to_decimal(neg_one_two_seven_2comp, 2));
	printf(" 127 is %4s\n", to_decimal(one_two_seven, 2));
	printf(" ? is %4s\n", to_decimal(min_2comp, 2));
	printf(" -15 is %4s\n", to_decimal(neg_fifteen, 2));
	printf("%d\n", x);
}
