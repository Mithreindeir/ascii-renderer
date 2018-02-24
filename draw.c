#include "draw.h"

struct text_buffer *text_buffer_init()
{
	struct text_buffer *buf = malloc(sizeof(struct text_buffer));

	buf->buf = NULL;
	buf->len = 0;
	buf->buf_size = 0;

	return buf;
}

void text_buffer_destroy(struct text_buffer *buf)
{
	if (!buf)
		return;

	free(buf->buf);
	free(buf);
}

void text_buffer_print(struct text_buffer *buf, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	size_t needed = vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	text_buffer_allocate(buf, needed);

	va_start(args, fmt);
	buf->len +=
	    vsnprintf(buf->buf + buf->len, buf->buf_size - buf->len, fmt, args);
	va_end(args);
}

void text_buffer_allocate(struct text_buffer *buf, int len)
{
	if ((buf->len + len) >= buf->buf_size) {
		buf->buf_size += len * 2;
		if (!buf->buf) {
			buf->buf = malloc(buf->buf_size);
		} else {
			buf->buf = realloc(buf->buf, buf->buf_size);
		}
	}
}

void text_buffer_render(struct text_buffer *buf)
{
	write_buffer(buf->buf, buf->len);
}

void text_buffer_clear(struct text_buffer *buf)
{
	buf->len = 0;
}

struct text_rect * text_rect_init(int x, int y, int w, int h)
{
    struct text_rect * rect = malloc(sizeof(struct text_rect));

    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    rect->fill = malloc(sizeof(char*)*h);
    for (int i = 0; i < h; i++) {
        rect->fill[i] = malloc(w);
        memset(rect->fill[i], 0, w);
    }

    return rect;
}

void text_wrap(char ** line_buf, int w, int h, const char * input)
{
    //Clear line buffer first
    for (int i = 0; i < h; i++) {
        memset(line_buf[i], 0, w);
    }
    int len = strlen(input);
    int col = 1, ln_last= 0, ln_iter = 0;
    int is_newline = 0;
    for (int i = 0; i < len; i++, col++) {
        if (input[i]=='\n' || col >= (w-1) || (i+1)==len) {
            is_newline = input[i]=='\n';
            strncpy(line_buf[ln_iter], input+ln_last, col-is_newline);
            i+=is_newline;
            ln_last = i;
            ln_iter++;
            col = 1;
        }
    }
}

void draw_rect(struct text_buffer *buf, struct text_rect *rect)
{
	int len = rect->w + 2;
	char *line = malloc(len + 1);
	line[len] = 0;

	for (int i = -1; i < rect->h + 1; i++) {
		text_buffer_print(buf, CURSOR_POS, rect->y + i, rect->x);
		if (i == -1 || i == rect->h) {
			memset(line, '-', len);
		} else {
			memset(line, 0x20, len);
			line[0] = '|';
			line[rect->w+1] = '|';
			strncpy(line+1, rect->fill[i], strlen(rect->fill[i]));
		}

		text_buffer_print(buf, "%s", line);
	}

	free(line);
}
