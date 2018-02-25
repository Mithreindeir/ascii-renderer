#ifndef ESCAPE_H
#define ESCAPE_H

/*Miscellaneous Escape Codes*/
#define ESC                 "\x1b"

#define SMOOTH_SCROLLING    "\x1b[?4H"
#define CLEAR_LINE          "\x1b[2K"
#define CLEAR_SCRN          "\x1b[2J"
#define CURSOR_REPORT       "\x1b[6n"
#define CURSOR_POS          "\x1b[%d;%df"
#define CURSOR_DOWN         "\x1b[%dB"
#define CURSOR_FORWARD      "\x1b[%dC"

/*Color Escape Codes*/
#define COLOR_24FG(r, g, b)     (ESC##"[38;2;"##r##";"##g##";"##b)
#define COLOR_4FG(c)             (ESC##"["##c##";0m"

#define BLACK               "30"
#define RED                 "31"
#define GREEN               "32"
#define YELLOW              "33"
#define BLUE                "34"
#define MAGENTA             "35"
#define CYAN                "36"
#define WHITE               "37"

#endif
