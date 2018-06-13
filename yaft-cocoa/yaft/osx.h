/* See LICENSE for licence details. */
/* osx.h: framebuffer struct/enum */
#ifndef _OSX_H_
#define _OSX_H_

/* initialize struct fb_info_t */
bool set_fbinfo(struct fb_info_t *info);

/* common framebuffer functions */
static inline uint32_t color2pixel(struct fb_info_t *info, uint32_t color);
void fb_print_info(struct fb_info_t *info);
static inline void draw_line(struct framebuffer_t *fb, struct terminal_t *term, int line);
bool fb_init(struct framebuffer_t *fb);
void fb_die(struct framebuffer_t *fb);
void fb_refresh(struct framebuffer_t *fb, struct terminal_t *term);

#endif /* _OSX_H_ */
