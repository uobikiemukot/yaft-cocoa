/* See LICENSE for licence details. */
#include "yaft.h"
#include "util.h"
#include "conf.h"
#include "osx.h"
#include "terminal.h"
#include "parse.h"

const uint8_t attr_mask[] = {
	0x00, 0x01, 0x00, 0x00, /* 0:none      1:bold  2:none 3:none */
	0x02, 0x04, 0x00, 0x08, /* 4:underline 5:blink 6:none 7:reverse */
};

const uint32_t bit_mask[] = {
	0x00,
	0x01,       0x03,       0x07,       0x0F,
	0x1F,       0x3F,       0x7F,       0xFF,
	0x1FF,      0x3FF,      0x7FF,      0xFFF,
	0x1FFF,     0x3FFF,     0x7FFF,     0xFFFF,
	0x1FFFF,    0x3FFFF,    0x7FFFF,    0xFFFFF,
	0x1FFFFF,   0x3FFFFF,   0x7FFFFF,   0xFFFFFF,
	0x1FFFFFF,  0x3FFFFFF,  0x7FFFFFF,  0xFFFFFFF,
	0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF,
};

volatile sig_atomic_t child_alive = false; /* SIGCHLD: child process (shell) is alive or not */
struct framebuffer_t fb;
struct terminal_t term;

/* yaft.c: include main function */
void sig_handler(int signo)
{
	/* global */
	extern volatile sig_atomic_t child_alive;

	logging(LOG_DEBUG, "caught signal! no:%d\n", signo);

	if (signo == SIGCHLD) {
		child_alive = false;
		wait(NULL);
	}
}

bool signal_init()
{
	struct sigaction sigact;

	memset(&sigact, 0, sizeof(struct sigaction));
	sigact.sa_handler = sig_handler;
	sigact.sa_flags   = SA_RESTART;
	esigaction(SIGCHLD, &sigact, NULL);

	return true;
}

void signal_die()
{
	struct sigaction sigact;

	memset(&sigact, 0, sizeof(struct sigaction));
	sigact.sa_handler = SIG_DFL;
	/* no error handling */
	sigaction(SIGCHLD, &sigact, NULL);
}

bool fork_and_exec(int *master, int lines, int cols)
{
	extern const char *shell_cmd; /* defined in conf.h */
	char *shell_env;
	pid_t pid;
	struct winsize ws = {.ws_row = lines, .ws_col = cols,
		/* XXX: this variables are UNUSED (man tty_ioctl),
			but useful for calculating terminal cell size */
		.ws_ypixel = CELL_HEIGHT * lines, .ws_xpixel = CELL_WIDTH * cols};

	pid = eforkpty(master, NULL, NULL, &ws);
	if (pid < 0)
		return false;
	else if (pid == 0) { /* child */
		esetenv("TERM", term_name, 1);
		if ((shell_env = getenv("SHELL")) != NULL)
			eexecl(shell_env);
		else
			eexecl(shell_cmd);
		/* never reach here */
		exit(EXIT_FAILURE);
	}
	return true;
}

int check_fds(fd_set *fds, struct timespec *ts, int master)
{
	FD_ZERO(fds);
	FD_SET(master, fds);
	ts->tv_sec  = 0;
	ts->tv_nsec = SELECT_TIMEOUT * 1000;
	return epselect(master + 1, fds, NULL, NULL, ts, NULL);
}

bool c_init(int width, int height)
{
	/* global */
	extern volatile sig_atomic_t child_alive;
	extern struct framebuffer_t fb;
	extern struct terminal_t term;

	/* init */
	if (setlocale(LC_ALL, "ja_JP.UTF-8") == NULL) /* for wcwidth() */
		logging(LOG_WARN, "setlocale falied\n");

	if (!fb_init(&fb, width, height)) {
		logging(LOG_FATAL, "framebuffer initialize failed\n");
		goto fb_init_failed;
	}

	if (!term_init(&term, width, height)) {
		logging(LOG_FATAL, "terminal initialize failed\n");
		goto term_init_failed;
	}

	if (!signal_init()) {
		logging(LOG_FATAL, "tty initialize failed\n");
		goto signal_init_failed;
	}

	/* fork and exec shell */
	if (!fork_and_exec(&term.fd, term.lines, term.cols)) {
		logging(LOG_FATAL, "forkpty failed\n");
		goto signal_init_failed;
	}
	child_alive = true;

	/* normal exit */
	return true;

	/* error exit */
signal_init_failed:
	term_die(&term);
term_init_failed:
	fb_die(&fb);
fb_init_failed:
	return false;
}

bool c_select()
{
	/* global*/
	extern volatile sig_atomic_t child_alive;
	extern struct framebuffer_t fb;
	extern struct terminal_t term;

	fd_set fds;
	struct timespec ts;
	uint8_t buf[BUFSIZE];
	size_t size;

	if (!child_alive)
		return false;

	if (check_fds(&fds, &ts, term.fd) == -1)
		return false;

	if (FD_ISSET(term.fd, &fds)
		&& (size = read(term.fd, buf, BUFSIZE)) > 0) {

		if (VERBOSE)
			ewrite(STDERR_FILENO, buf, size);

		parse(&term, buf, size);
		fb_refresh(&fb, &term);
		return true;
	}
	return false;
}

void c_write(const char *str, size_t size)
{
	/* global */
	extern struct terminal_t term;

	ewrite(term.fd, str, size);
}

uint32_t c_get_color(int i)
{
	/* global */
	extern const uint32_t color_list[NCOLORS];

	return color_list[i];
}
