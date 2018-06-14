#ifndef _GLYPH_H_
#define _GLYPH_H_

struct glyph_t {
	uint32_t code;
	uint8_t width;
	uint16_t bitmap[16];
};

enum {
	CELL_WIDTH = 8,
	CELL_HEIGHT = 16,
	NCHARS = 65534,
};

extern const struct glyph_t glyphs[NCHARS];

#endif /* _GLYPH_H_ */
