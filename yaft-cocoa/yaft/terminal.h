/* See LICENSE for licence details. */
/* terminal.h */
#ifndef _TERMINAL_H_
#define _TERMINAL_H_

int set_cell(struct terminal_t *term, int y, int x, const struct glyph_t *glyphp);
static inline void swap_lines(struct terminal_t *term, int i, int j);
const struct glyph_t *drcs_glyph(struct terminal_t *term, uint32_t code);
bool push_esc(struct terminal_t *term, uint8_t ch);
void reset_charset(struct terminal_t *term);
void add_char(struct terminal_t *term, uint32_t code);
void reset_esc(struct terminal_t *term);
void term_scroll(struct terminal_t *term, int from, int to, int offset);
void erase_cell(struct terminal_t *term, int y, int x);
void copy_cell(struct terminal_t *term, int dst_y, int dst_x, int src_y, int src_x);
void reset(struct terminal_t *term);
// relative movement: cause scrolling
void move_cursor(struct terminal_t *term, int y_offset, int x_offset);
// absolute movement: never scroll
void set_cursor(struct terminal_t *term, int y, int x);

bool term_init(struct terminal_t *term, int width, int height);
void redraw(struct terminal_t *term);
void term_die(struct terminal_t *term);

#endif /* _TERMINAL_H_ */
