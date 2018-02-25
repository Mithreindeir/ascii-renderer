#include <stdio.h>
#include <stdlib.h>
#include "draw.h"
#include "syntax.h"

#define CTRL_KEY(k) ((k) & 0x1f)

int main(int argc, char **argv)
{
    struct termios original = get_term();
	struct text_buffer *buf = text_buffer_init();
	int w, h;
	get_winsize(&w, &h);
    struct text_rect * rect = text_rect_init(1, 2, w/2-2, h/2-2);
    struct text_rect * rect2 = text_rect_init(w/2+1, 2, w/2-2, h/2-2);
    set_term(raw_term());

	char c = 0;
	char cbuf[400];
	int iter=0;
	memset(cbuf, 0, 400);
	do {
		write_str("\x1b[?25l");
		write_str("\x1b[2J");
		write_str("\x1b[H");
		text_buffer_clear(buf);
    
	    text_wrap(rect->fill, rect->w, rect->h, cbuf);
	    text_wrap(rect2->fill, rect2->w, rect2->h, cbuf);
	    draw_rect(buf, rect2);
		draw_rect(buf, rect);
		
		text_buffer_print(buf, "\x1b[?25h");
		text_buffer_render(buf);
		c = read_char();
		if (iter >= 400) iter = 0;
        if ((c == CTRL_KEY('H') || c == 0x7f) && iter >=1) {
            iter--;
            memmove(cbuf+iter, cbuf+iter+1, strlen(cbuf)-iter);
        }
		if (c>=0x20&&c<0x7f)
		    cbuf[iter++] = c;
	} while (c != CTRL_KEY('q'));

	text_buffer_destroy(buf);
	set_term(original);
	return 0;
}
