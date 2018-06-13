struct glyph_t {
	uint32_t code;
	uint8_t width;
	uint16_t bitmap[16];
};

enum {
	CELL_WIDTH = 8,
	CELL_HEIGHT = 16
};
