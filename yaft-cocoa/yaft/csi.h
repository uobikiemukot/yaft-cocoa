/* See LICENSE for licence details. */
/* csi.h: function for csi sequence */
#ifndef _CSI_H_
#define _CSI_H_

void insert_blank(struct terminal_t *term, struct parm_t *parm);
void curs_up(struct terminal_t *term, struct parm_t *parm);
void curs_down(struct terminal_t *term, struct parm_t *parm);
void curs_forward(struct terminal_t *term, struct parm_t *parm);
void curs_back(struct terminal_t *term, struct parm_t *parm);
void curs_nl(struct terminal_t *term, struct parm_t *parm);
void curs_pl(struct terminal_t *term, struct parm_t *parm);
void curs_col(struct terminal_t *term, struct parm_t *parm);
void curs_pos(struct terminal_t *term, struct parm_t *parm);
void curs_line(struct terminal_t *term, struct parm_t *parm);
void erase_display(struct terminal_t *term, struct parm_t *parm);
void erase_line(struct terminal_t *term, struct parm_t *parm);
void insert_line(struct terminal_t *term, struct parm_t *parm);
void delete_line(struct terminal_t *term, struct parm_t *parm);
void delete_char(struct terminal_t *term, struct parm_t *parm);
void erase_char(struct terminal_t *term, struct parm_t *parm);
uint8_t rgb2index(uint8_t r, uint8_t g, uint8_t b);
void set_attr(struct terminal_t *term, struct parm_t *parm);
void status_report(struct terminal_t *term, struct parm_t *parm);
void device_attribute(struct terminal_t *term, struct parm_t *parm);
void set_mode(struct terminal_t *term, struct parm_t *parm);
void reset_mode(struct terminal_t *term, struct parm_t *parm);
void set_margin(struct terminal_t *term, struct parm_t *parm);
void clear_tabstop(struct terminal_t *term, struct parm_t *parm);

#endif /* _CSI_H_ */
