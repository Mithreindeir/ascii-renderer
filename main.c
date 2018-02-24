#include <stdio.h>
#include <stdlib.h>
#include "draw.h"

int main(int argc, char **argv)
{
	struct termios original = get_term();
	struct text_buffer *buf = text_buffer_init();
	set_term(raw_term());

	char c = 0;
	while (c != 'q') {
		write_str("\x1b[?25l");
		write_str("\x1b[2J");
		write_str("\x1b[H");
		text_buffer_print(buf, "(%c)", c);
		if (c == 'c')
			text_buffer_clear(buf);
		text_buffer_print(buf, "\x1b[?25h");
		text_buffer_render(buf);
		c = read_char();
	}

	text_buffer_destroy(buf);
	set_term(original);
	return 0;
}
