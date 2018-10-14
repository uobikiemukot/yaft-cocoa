/* See LICENSE for licence details. */
/* function for osc sequence */
#ifndef _OSC_H_
#define _OSC_H_

int32_t parse_color1(char *seq);
int32_t parse_color2(char *seq);
void set_palette(struct terminal_t *term, void *arg);
void reset_palette(struct terminal_t *term, void *arg);
int isdigit_or_questionmark(int c);
void glyph_width_report(struct terminal_t *term, void *arg);

#endif /* _OSC_H_ */
