/* See LICENSE for licence details. */
#ifndef _COLOR_H_
#define _COLOR_H_

/*
	Standard VGA colors
	http://en.wikipedia.org/wiki/ANSI_escape_code

	xterm 256color
	http://www.calmar.ws/vim/256-xterm-24bit-rgb-color-chart.html
	http://jonasjacek.github.io/colors/

	byte order: (MSB) RR GG BB (LSB)
*/
enum {
	NCOLORS      = 256, /* number of color palette */
	BITS_PER_RGB = 8,
};

extern const uint32_t color_list[NCOLORS];

#endif /* _COLOR_H_ */
