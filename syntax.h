#ifndef SYNTAX_H
#define SYNTAX_H

#include <string.h>
#include "draw.h"

/*Very simple syntax coloring*/
struct color_pair {
    char * keyword;
    int len;
    int color;
};

struct color_pair color_pair_init(char * keyword, char * color);

void str_append(char ** str1, int *len1, const char * str2, int len2);
char * color_buffer(const char * input, int len, struct color_pair * pairs, int num_pairs);
char * color_text(const char * string, const char * color_buf, int len);

#endif
