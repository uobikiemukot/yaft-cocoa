/* See LICENSE for licence details. */
/* esc.h: c0 controll char and escape sequence */
#ifndef _ESC_H_
#define _ESC_H_

/* function for control character */
void bs(struct terminal_t *term);
void tab(struct terminal_t *term);
void newline(struct terminal_t *term);
void cr(struct terminal_t *term);
void enter_esc(struct terminal_t *term);

/* function for escape sequence */
void save_state(struct terminal_t *term);
void restore_state(struct terminal_t *term);
void crnl(struct terminal_t *term);
void set_tabstop(struct terminal_t *term);
void reverse_nl(struct terminal_t *term);
void identify(struct terminal_t *term);
void enter_csi(struct terminal_t *term);
void enter_osc(struct terminal_t *term);
void enter_dcs(struct terminal_t *term);
void ris(struct terminal_t *term);

#endif /* _ESC_H_ */
