/* os specific ioctl (dummy) */
#include "yaft.h"
#include "conf.h"
#include "util.h"
#include "osx.h"

/* initialize struct fb_info_t */
bool set_fbinfo(struct fb_info_t *info)
{
	info->bytes_per_pixel = BITS_PER_PIXEL / BITS_PER_BYTE;
	info->bits_per_pixel  = BITS_PER_PIXEL;
	info->screen_size = info->width * info->height * info->bytes_per_pixel;
	info->line_length = info->width * info->bytes_per_pixel;

	info->red   = (struct bitfield_t){.length = 8, .offset = 0};
	info->green = (struct bitfield_t){.length = 8, .offset = 8};
	info->blue  = (struct bitfield_t){.length = 8, .offset = 16};
	info->alpha = (struct bitfield_t){.length = 8, .offset = 24};

	return true;
}

/* common framebuffer functions */
static inline uint32_t color2pixel(struct fb_info_t *info, uint32_t color)
{
	uint32_t r, g, b;

	r = bit_mask[BITS_PER_RGB] & (color >> (BITS_PER_RGB * 2));
	g = bit_mask[BITS_PER_RGB] & (color >>  BITS_PER_RGB);
	b = bit_mask[BITS_PER_RGB] & (color >>  0);

	r = r >> (BITS_PER_RGB - info->red.length);
	g = g >> (BITS_PER_RGB - info->green.length);
	b = b >> (BITS_PER_RGB - info->blue.length);

	return (r << info->red.offset)
		+ (g << info->green.offset)
		+ (b << info->blue.offset)
		// alpha is always 0xFF
		+ (0xFF << info->alpha.offset);
}

void fb_print_info(struct fb_info_t *info)
{
	logging(LOG_DEBUG, "framebuffer info:\n");
	logging(LOG_DEBUG, "\tred(off:%d len:%d) green(off:%d len:%d) blue(off:%d len:%d)\n",
		info->red.offset, info->red.length, info->green.offset, info->green.length, info->blue.offset, info->blue.length);
	logging(LOG_DEBUG, "\tresolution %dx%d\n", info->width, info->height);
	logging(LOG_DEBUG, "\tscreen size:%ld line length:%d\n", info->screen_size, info->line_length);
	logging(LOG_DEBUG, "\tbits_per_pixel:%d bytes_per_pixel:%d\n", info->bits_per_pixel, info->bytes_per_pixel);
}

bool fb_init(struct framebuffer_t *fb, int width, int height)
{
	/* os dependent initialize */
	fb->info.width  = width;
	fb->info.height = height;

	if (!set_fbinfo(&fb->info))
		goto set_fbinfo_failed;

	if (VERBOSE)
		fb_print_info(&fb->info);

	/* allocate memory */
	fb->buf  = (uint8_t *) ecalloc(1, fb->info.screen_size);
	memset(fb->buf, 0xFF, fb->info.screen_size);

	/* error check */
	if (!fb->buf)
		goto allocate_failed;

	/* init color palette */
	for (int i = 0; i < NCOLORS; i++)
		fb->real_palette[i] = color2pixel(&fb->info, color_list[i]);

	return true;

allocate_failed:
	free(fb->buf);
set_fbinfo_failed:
	return false;
}

void fb_die(struct framebuffer_t *fb)
{
	free(fb->buf);
}

static inline void draw_line(struct framebuffer_t *fb, struct terminal_t *term, int line)
{
	int pos, bdf_padding, glyph_width, margin_right;
	int col, w, h;
	uint32_t pixel;
	struct color_pair_t color_pair;
	struct cell_t *cellp;

	for (col = term->cols - 1; col >= 0; col--) {
		margin_right = (term->cols - 1 - col) * CELL_WIDTH;

		/* target cell */
		cellp = &term->cells[line][col];

		/* copy current color_pair (maybe changed) */
		color_pair = cellp->color_pair;

		/* check wide character or not */
		glyph_width = (cellp->width == HALF) ? CELL_WIDTH: CELL_WIDTH * 2;
		bdf_padding = my_ceil(glyph_width, BITS_PER_BYTE) * BITS_PER_BYTE - glyph_width;
		if (cellp->width == WIDE)
			bdf_padding += CELL_WIDTH;

		/* check cursor positon */
		if ((term->mode & MODE_CURSOR && line == term->cursor.y)
			&& (col == term->cursor.x
			|| (cellp->width == WIDE && (col + 1) == term->cursor.x)
			|| (cellp->width == NEXT_TO_WIDE && (col - 1) == term->cursor.x))) {
			color_pair.fg = DEFAULT_BG;
			color_pair.bg = ACTIVE_CURSOR_COLOR;
		}

		for (h = 0; h < CELL_HEIGHT; h++) {
			/* if UNDERLINE attribute on, swap bg/fg */
			if ((h == (CELL_HEIGHT - 1)) && (cellp->attribute & attr_mask[ATTR_UNDERLINE]))
				color_pair.bg = color_pair.fg;

			for (w = 0; w < CELL_WIDTH; w++) {
				pos = (term->width - 1 - margin_right - w) * fb->info.bytes_per_pixel
					+ (line * CELL_HEIGHT + h) * fb->info.line_length;

				/* set color palette */
				if (cellp->glyphp->bitmap[h] & (0x01 << (bdf_padding + w)))
					pixel = fb->real_palette[color_pair.fg];
				else
					pixel = fb->real_palette[color_pair.bg];

				/* update copy buffer only */
				memcpy(fb->buf + pos, &pixel, fb->info.bytes_per_pixel);
			}
		}
	}

	/* actual display update (bit blit) */
	/*
	pos = (line * CELL_HEIGHT) * fb->info.line_length;
	size = CELL_HEIGHT * fb->info.line_length;
	memcpy(fb->fp + pos, fb->buf + pos, size);
	*/

	/* TODO: page flip
		if fb_fix_screeninfo.ypanstep > 0, we can use hardware panning.
		set fb_fix_screeninfo.{yres_virtual,yoffset} and call ioctl(FBIOPAN_DISPLAY)
		but drivers  of recent hardware (inteldrmfb, nouveaufb, radeonfb) don't support...
		(maybe we can use this by using libdrm) */
	/* TODO: vertical synchronizing */

	term->line_dirty[line] = ((term->mode & MODE_CURSOR) && term->cursor.y == line) ? true: false;
}

void fb_refresh(struct framebuffer_t *fb, struct terminal_t *term)
{
	if (term->mode & MODE_CURSOR)
		term->line_dirty[term->cursor.y] = true;

	for (int line = 0; line < term->lines; line++) {
		if (term->line_dirty[line]) {
			draw_line(fb, term, line);
		}
	}
}
