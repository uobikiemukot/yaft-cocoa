/* See LICENSE for licence details. */
/* parse.h: char/esc sequence/charset function */
#ifndef _PARSE_H_
#define _PARSE_H_

/*
void control_character(struct terminal_t *term, uint8_t ch);
void esc_sequence(struct terminal_t *term, uint8_t ch);
void csi_sequence(struct terminal_t *term, uint8_t ch);
int is_osc_parm(int c);
void omit_string_terminator(char *bp, uint8_t ch);
void osc_sequence(struct terminal_t *term, uint8_t ch);
void dcs_sequence(struct terminal_t *term, uint8_t ch);
void utf8_charset(struct terminal_t *term, uint8_t ch);
*/
void parse(struct terminal_t *term, uint8_t *buf, size_t size);

#endif /* _PARSE_H_ */
