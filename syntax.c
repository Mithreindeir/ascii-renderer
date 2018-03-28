#include "syntax.h"

struct color_pair color_pair_init(char *keyword, char *color)
{
	struct color_pair cp;

	int len = strlen(keyword);
	cp.keyword = malloc(len + 1);

	for (int i = 0; i < len; i++)
		cp.keyword[i] = keyword[i];

	cp.keyword[len] = 0;

	cp.len = strlen(keyword);
	cp.color = strtol(color, NULL, 10);

	return cp;
}

void color_buffer(const char *input, char *color_buf, int len, struct color_pair *pairs, int num_pairs)
{
	if (!input || !pairs || len == 0)
		return;
	int esc_len = 0;
	int clen = 0;
	for (int j = 0; j < len; j++) {
		if (color_buf[j]!=0) continue;
		if ((esc_len=escape_code(input+j, len-j)) > 0) {
			j += esc_len-1;
			continue;
		}
		for (int i = 0; i < num_pairs; i++) {
			char const**saved = NULL;
			int slen=0;
			if ((clen = dmatch_text(input + j, pairs[i].keyword,&saved,&slen))) {
				if (slen<=1) {
					writef("Matching error: <= 1 marker found at \"%s\"\r\n", pairs[i].keyword);
					//exit(1);
					continue;
				}
				int st = saved[0]-(input+j);
				int et = saved[1]-(input+j);
				memset(color_buf+j+st, pairs[i].color, et-st);
				j += et-1;
				free(saved);
				break;
			}
			free(saved);
		}
	}
}

struct color_pair *color_pair_add(struct color_pair *pairs, int *num_pairs, struct color_pair pair)
{
	struct color_pair * p = pairs;
	int np = *num_pairs + 1;
	if (p) {
		p = realloc(p, np*sizeof(struct color_pair));
	} else {
		p = malloc(np*sizeof(struct color_pair));
	}
	p[np-1] = pair;
	*num_pairs = np;
	return p;
}
