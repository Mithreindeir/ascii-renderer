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

void draw_rect(struct text_buffer *buf, struct text_rect *rect)
{
	int len = rect->w + 2;
	char *line = malloc(len + 1);
	line[len] = 0;

	for (int i = -1; i < rect->h + 1; i++) {
		text_buffer_print(buf, CURSOR_POS, i + rect->x, j + rect->y);
		if (i == -1 || i == rect->h) {
			memset(line, '-', len);
		} else {
			memset(line, 0x20, len);
			line[0] = '|';
			line[rect->w] = '|';
			strncpy(line, rect->fill[i], strlen(rect->fill[i]));
		}
	}

	free(line);
}
