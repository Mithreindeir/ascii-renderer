#include <stdio.h>
#include <stdlib.h>
#include "draw.h"
#include "syntax.h"

#define CTRL_KEY(k) ((k) & 0x1f)

struct color_pair *cpairs;
int num_cpairs;
struct text_buffer *buf;
struct text_rect *rect;
char cbuf[400];

struct color_pair *color_init(int *num_pairs)
{
	struct color_pair *pairs = NULL;
	
	pairs = color_pair_add(pairs, num_pairs, color_pair_init("mov", CYAN));
	pairs = color_pair_add(pairs, num_pairs, color_pair_init("add", YELLOW));
	pairs = color_pair_add(pairs, num_pairs, color_pair_init("0", GREEN));
	pairs = color_pair_add(pairs, num_pairs, color_pair_init("push", MAGENTA));
	pairs = color_pair_add(pairs, num_pairs, color_pair_init("pop", MAGENTA));

	return pairs;
}

void color_destroy(struct color_pair *pairs, int num_pairs)
{
	if (!pairs || !num_pairs) return;
	
	for (int i = 0; i < num_pairs; i++) {
		free(pairs[i].keyword);
	}

	free(pairs);
}

void render()
{
	text_wrap(rect->fill, rect->w, rect->h, cbuf);
	draw_rect(buf, rect);
	text_buffer_render(buf);
}

void loop()
{
	char c;
	int iter = 0;
	memset(cbuf, 0, 400);
	do {
		write_str("\x1b[?25l");
		write_str("\x1b[2J");
		write_str("\x1b[H");
		
		text_buffer_clear(buf);
		render();

		write_str("\x1b[?25h");

		c = read_char();
		if (iter >= 400)
			iter = 0;
		if (c >= 0x20 && c < 0x7f)
			cbuf[iter++] = c;
		if ((c==CTRL_KEY('H') || c == 0x7f) && iter >= 1) {
			iter--;
			memmove(cbuf+iter, cbuf+iter+1, strlen(cbuf)-iter);
			cbuf[iter] = 0;
		}
	} while (c != CTRL_KEY('q'));
}

int main(int argc, char **argv)
{
	struct termios original = get_term();
	set_term(raw_term());

	num_cpairs = 0;
	cpairs = color_init(&num_cpairs);
	buf = text_buffer_init();
	buf->num_pairs = num_cpairs;
	buf->pairs = cpairs;
	rect = text_rect_init(1, 2, 20, 20);

	loop();

	color_destroy(cpairs, num_cpairs);
	text_rect_destroy(rect);
	text_buffer_destroy(buf);
	set_term(original);
	return 0;
}
