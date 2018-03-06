#ifndef SYNTAX_H
#define SYNTAX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "escape.h"
#include "../dmatch.h"

/*Very simple syntax coloring*/
struct color_pair {
	char *keyword;
	int len;
	int color;
};

struct color_pair color_pair_init(char *keyword, char *color);
char *color_buffer(const char *input, int len, struct color_pair *pairs,
		int num_pairs);
//Helpers
struct color_pair *color_pair_add(struct color_pair * pairs, int *num_pairs,
		struct color_pair pair);

#endif
