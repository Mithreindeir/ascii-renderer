#include "syntax.h"

struct color_pair color_pair_init(char * keyword, char * color)
{
    struct color_pair cp;
    
    int len = strlen(keyword);
    cp.keyword = malloc(len+1);
    
    for (int i = 0; i < len; i++)
        cp.keyword[i] = keyword[i];
    
    cp.keyword[len] = 0;

    cp.len = strlen(keyword);
    cp.color = strtol(color, NULL, 10);

    return cp;
}

void str_append(char ** str1, int * len1, const char * str2, int len2)
{
    if (!str2 || !len2) return;

    char * str = *str1;
    int os = *len1;
    *len1 += len2;
    if (!str) {
        str = malloc(*len1+1);
    } else {
        str = realloc(str, *len1+1);
    }

    strncpy(str+os, str2, len2);
    str[*len1] = 0;
    *str1 = str;
}

char * color_buffer(const char * input, int len, struct color_pair * pairs, int num_pairs)
{
    if (!input || !pairs || len == 0) return NULL;
    
    char * color_buf = malloc(len);
    memset(color_buf, 0, len);
   
    for (int j = 0; j < len; j++) {
        for (int i = 0; i < num_pairs; i++) {
            if ((j+pairs[i].len) >= len) continue;
            if (!strncmp(input + j, pairs[i].keyword, pairs[i].len)) {
                memset(color_buf+j, pairs[i].color, pairs[i].len);
                j += pairs[i].len-1;
                break;
            }
        }
    }

    return color_buf;
}

char * color_text(const char * string, const char * color_buf, int len)
{
    if (!string || !color_buf) return NULL;

    char * cstr = NULL;
    int clen = 0;

    int color_start = 0;
    int color = -1;

    char buf[16];
    memset(buf, 0, 16);

    for (int i = 0; i < len; i++) {
        int eob = (i+1) >= len;
        if (color != color_buf[i])  {
            color = color_buf[i];
            if ((i - color_start) > 0 || eob)
                str_append(&cstr, &clen, string+color_start, i-color_start+eob);
            int cl = snprintf(buf, 16, "\x1b[%dm", color_buf[i]);
            str_append(&cstr, &clen, buf, cl);
            color_start = i;
        } else if (eob) {
            str_append(&cstr, &clen, string+color_start, i-color_start+1);
        }
    }

    return cstr;
}
