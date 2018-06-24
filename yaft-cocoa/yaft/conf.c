/* See LICENSE for licence details. */
/* conf.h: define custom variables */

/* TERM value */
const char *term_name = "yaft-256color";

/* framubuffer device */
#if defined(__linux__)
        const char *fb_path = "/dev/fb0";
#elif defined(__FreeBSD__)
        const char *fb_path = "/dev/ttyv0";
#elif defined(__NetBSD__)
        const char *fb_path = "/dev/ttyE0";
#elif defined(__OpenBSD__)
        const char *fb_path = "/dev/ttyC0";
#elif defined(__ANDROID__)
        const char *fb_path = "/dev/graphics/fb0";
#endif

/* shell: refer SHELL environment variable at first */
#if defined(__linux__) || defined(__MACH__)
        const char *shell_cmd = "/bin/bash";
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
        const char *shell_cmd = "/bin/csh";
#elif defined(__ANDROID__)
        const char *shell_cmd = "/system/bin/sh";
#endif
