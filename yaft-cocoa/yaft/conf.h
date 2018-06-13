/* See LICENSE for licence details. */
/* conf.h: define custom variables */
#ifndef _CONF_H_
#define _CONF_H_

/* color: index number of color_palette[] (see color.h) */
enum {
	DEFAULT_FG           = 7,
	DEFAULT_BG           = 0,
	ACTIVE_CURSOR_COLOR  = 2,
	PASSIVE_CURSOR_COLOR = 1,
};

/* misc */
enum {
	VERBOSE          = false,   /* write dump of input to stdout, debug message to stderr */
	TABSTOP          = 8,      /* hardware tabstop */
	LAZY_DRAW        = true,   /* don't draw when input data size is larger than BUFSIZE */
	BACKGROUND_DRAW  = false,  /* always draw even if vt is not active */
	VT_CONTROL       = true,   /* handle vt switching */
	FORCE_TEXT_MODE  = false,  /* force KD_TEXT mode (not use KD_GRAPHICS mode) */
	SUBSTITUTE_HALF  = 0xFFFD, /* used for missing glyph(single width): U+FFFD (REPLACEMENT CHARACTER)) */
	SUBSTITUTE_WIDE  = 0x3013, /* used for missing glyph(double width): U+3013 (GETA MARK) */
	REPLACEMENT_CHAR = 0xFFFD, /* used for malformed UTF-8 sequence   : U+FFFD (REPLACEMENT CHARACTER)  */
};

/* TERM value */
extern const char *term_name;

/* framubuffer device */
extern const char *fb_path;

/* shell: refer SHELL environment variable at first */
extern const char *shell_cmd;

#endif /* _CONF_H_ */
