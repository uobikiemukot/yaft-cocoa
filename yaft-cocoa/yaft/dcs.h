/* See LICENSE for licence details. */
/* dcs.h: function for dcs sequence */
#ifndef _DCS_H_
#define _DCS_H_

enum {
	RGBMAX = 255,
	HUEMAX = 360,
	LSMAX  = 100,
};

/*
static inline void split_rgb(uint32_t color, uint8_t *r, uint8_t *g, uint8_t *b);
*/
static inline int sixel_bitmap(struct terminal_t *term, struct sixel_canvas_t *sc, uint8_t bitmap);
static inline uintptr_t sixel_repeat(struct terminal_t *term, struct sixel_canvas_t *sc, char *buf);
static inline uintptr_t sixel_attr(struct sixel_canvas_t *sc, char *buf);
static inline uint32_t hue2rgb(int n1, int n2, int hue);
static inline uint32_t hls2rgb(int hue, int lum, int sat);
static inline uintptr_t sixel_color(struct sixel_canvas_t *sc, char *buf);
static inline int sixel_cr(struct sixel_canvas_t *sc);
static inline int sixel_nl(struct sixel_canvas_t *sc);
void sixel_parse_data(struct terminal_t *term, struct sixel_canvas_t *sc, char *start_buf);
void reset_sixel(struct sixel_canvas_t *sc, struct color_pair_t color_pair, int width, int height);
void sixel_copy2cell(struct terminal_t *term, struct sixel_canvas_t *sc);
void sixel_parse_header(struct terminal_t *term, char *start_buf);
static inline void decdld_bitmap(struct glyph_t *glyph, uint8_t bitmap, uint8_t row, uint8_t column);
static inline void init_glyph(struct glyph_t *glyph);
void decdld_parse_data(char *start_buf, int start_char, struct glyph_t *chars);
void decdld_parse_header(struct terminal_t *term, char *start_buf);

#endif /* _DCS_H_ */
