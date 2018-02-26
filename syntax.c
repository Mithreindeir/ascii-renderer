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

char *color_buffer(const char *input, int len, struct color_pair *pairs,
		   int num_pairs)
{
	if (!input || !pairs || len == 0)
		return NULL;

	int esc_len = 0;
	char *color_buf = malloc(len);
	memset(color_buf, 0, len);

	for (int j = 0; j < len; j++) {
		if ((esc_len=escape_code(input+j, len-j)) > 0) {
			j += esc_len-1;
			continue;
		}
		for (int i = 0; i < num_pairs; i++) {
			if ((j + pairs[i].len) >= len)
				continue;
			if (!strncmp(input + j, pairs[i].keyword, pairs[i].len)) {
				memset(color_buf + j, pairs[i].color,
				       pairs[i].len);
				j += pairs[i].len - 1;
				break;
			}
		}
	}

	return color_buf;
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



